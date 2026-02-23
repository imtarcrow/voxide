#include "shader_program.hpp"

#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>

ShaderProgram::ShaderProgram(std::string vertex_shader_path, std::string fragment_shader_path)
    : vertex_shader_path(std::move(vertex_shader_path))
    , fragment_shader_path(std::move(fragment_shader_path))
{
    if (!this->load_and_compile()) {
        spdlog::error("Failed to construct Shader Program");
        throw std::runtime_error("Failed to construct Shader Program");
    }
}

ShaderProgram::~ShaderProgram()
{
    if (this->gl_program_handle != 0) {
        glDeleteProgram(this->gl_program_handle);
        this->gl_program_handle = 0;
    }
}

auto ShaderProgram::load_shader_file(const std::string& path) -> std::optional<std::string>
{
    std::ifstream file(path);

    if (!file.is_open()) {
        return std::nullopt;
    }

    std::ostringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}

auto ShaderProgram::create_shader(const std::string& filepath, int shader_type) -> std::optional<GLuint>
{
    GLuint shader = glCreateShader(shader_type);
    if (shader == 0) {
        return 0;
    }

    std::optional<std::string> shader_code = ShaderProgram::load_shader_file(filepath);

    if (!shader_code.has_value()) {
        return 0;
    }

    const char* code = shader_code->c_str();
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::string log(log_length, '\0');
        glGetShaderInfoLog(shader, log_length, nullptr, log.data());

        glDeleteShader(shader);
        return {};
    }

    return shader;
}

auto ShaderProgram::load_and_compile() -> bool
{
    GLuint shader_program = glCreateProgram();
    if (shader_program == 0) {
        return false;
    }

    std::optional<GLuint> vertex_shader = ShaderProgram::create_shader(this->vertex_shader_path, GL_VERTEX_SHADER);
    std::optional<GLuint> fragment_shader = ShaderProgram::create_shader(this->fragment_shader_path, GL_FRAGMENT_SHADER);

    if (!vertex_shader.has_value() || !fragment_shader.has_value()) {
        return false;
    }

    glAttachShader(shader_program, vertex_shader.value());
    glAttachShader(shader_program, fragment_shader.value());

    glLinkProgram(shader_program);

    GLint success = GL_FALSE;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        GLint log_length = 0;
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(log_length, '\0');
        glGetProgramInfoLog(shader_program, log_length, nullptr, log.data());
        return false;
    }

    this->gl_program_handle = shader_program;

    glDeleteShader(vertex_shader.value());
    glDeleteShader(fragment_shader.value());

    return true;
}

[[nodiscard]] auto ShaderProgram::use() const -> bool
{
    if (this->gl_program_handle == 0) {
        return false;
    }

    glUseProgram(this->gl_program_handle);
    return true;
}

[[nodiscard]] auto ShaderProgram::reload() -> bool
{
    GLuint old_program_id = this->gl_program_handle;

    assert(this->gl_program_handle != 0);

    if (!this->load_and_compile()) {
        return false;
    }

    glDeleteProgram(old_program_id);
    return true;
}
