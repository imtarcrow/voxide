#include "shader_program.hpp"

#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>

#include "glad/glad.h"

ShaderProgram::ShaderProgram(std::string vertex_shader_path, std::string fragment_shader_path)
    : vertex_shader_path(std::move(vertex_shader_path))
    , fragment_shader_path(std::move(fragment_shader_path))
{
    spdlog::trace("Creating ShaderProgram: vs={}, fs={}", this->vertex_shader_path, this->fragment_shader_path);

    if (!this->load_and_compile()) {
        spdlog::error("Failed to load and compile ShaderProgram");
        throw std::runtime_error("Failed to load and compile ShaderProgram");
    }
}

auto ShaderProgram::load_and_compile() -> bool
{
    GLuint program_handle = glCreateProgram();
    if (program_handle == 0) {
        spdlog::error("glCreateProgram() returned 0");
        return false;
    }

    std::optional<GLuint> vertex_shader = ShaderProgram::create_shader(this->vertex_shader_path, GL_VERTEX_SHADER);
    if (!vertex_shader.has_value()) {
        return false;
    }

    std::optional<GLuint> fragment_shader = ShaderProgram::create_shader(this->fragment_shader_path, GL_FRAGMENT_SHADER);
    if (!fragment_shader.has_value()) {
        glDeleteShader(vertex_shader.value());
        return false;
    }

    glAttachShader(program_handle, vertex_shader.value());
    glAttachShader(program_handle, fragment_shader.value());

    glLinkProgram(program_handle);

    GLint success = GL_FALSE;
    glGetProgramiv(program_handle, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        GLint log_length = 0;
        glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(log_length, '\0');
        glGetProgramInfoLog(program_handle, log_length, nullptr, log.data());

        spdlog::error("Failed to link shader program (vs={}, fs={}):\n{}", vertex_shader_path, fragment_shader_path, log);
        return false;
    }
    spdlog::debug("Successfully linked program: vs={}, fs={}", vertex_shader_path, fragment_shader_path);

    this->gl_handle = program_handle;

    glDeleteShader(vertex_shader.value());
    glDeleteShader(fragment_shader.value());

    return true;
}

auto ShaderProgram::create_shader(const std::string& filepath, int shader_type) -> std::optional<GLuint>
{
    GLuint shader_handle = glCreateShader(shader_type);
    if (shader_handle == 0) {
        spdlog::error("glCreateShader({}) returned 0", shader_type);
        return std::nullopt;
    }
    spdlog::trace("glCreateShader({}) -> handle={}", shader_type, shader_handle);

    std::optional<std::string> shader_code = ShaderProgram::load_shader_file(filepath);

    if (!shader_code.has_value()) {
        spdlog::error("Cannot load shader file: {}", filepath);
        glDeleteShader(shader_handle);
        return std::nullopt;
    }

    const char* c_code = shader_code->c_str();
    glShaderSource(shader_handle, 1, &c_code, nullptr);
    glCompileShader(shader_handle);

    GLint success = GL_FALSE;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        GLint log_length = 0;
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_length);
        std::string log(log_length, '\0');
        glGetShaderInfoLog(shader_handle, log_length, nullptr, log.data());

        spdlog::error("Failed to compile shader ({}):\n{}", filepath, log);

        glDeleteShader(shader_handle);
        return std::nullopt;
    }
    spdlog::debug("Successfully compiled shader: {}", filepath);

    return shader_handle;
}

auto ShaderProgram::load_shader_file(const std::string& filepath) -> std::optional<std::string>
{
    std::ifstream file(filepath);

    if (!file.is_open()) {
        spdlog::error("Cannot open shader file: {}", filepath);
        return std::nullopt;
    }

    std::ostringstream stream;
    stream << file.rdbuf();
    file.close();

    spdlog::trace("Loaded shader source: {}", filepath);
    return stream.str();
}

ShaderProgram::~ShaderProgram()
{
    spdlog::trace("Destroying ShaderProgram {}", this->gl_handle);
    if (this->gl_handle != 0) {
        glDeleteProgram(this->gl_handle);
        this->gl_handle = 0;
    }
}

[[nodiscard]] auto ShaderProgram::use() const -> bool
{
    if (this->gl_handle == 0) {
        return false;
    }

    glUseProgram(this->gl_handle);

    return true;
}

auto ShaderProgram::reload() -> bool
{

    spdlog::info("Hot-reloading ShaderProgram {}", this->gl_handle);

    assert(this->gl_handle != 0);
    GLuint old_handle = this->gl_handle;

    if (!this->load_and_compile()) {
        spdlog::error("ShaderProgram reload failed, keeping old program");
        return false;
    }

    spdlog::info("ShaderProgram reloaded successfully (new handle={})", this->gl_handle);
    
    glDeleteProgram(old_handle);
    return true;
}
