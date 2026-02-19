#include "shader_program.hpp"

#include <stdexcept>

ShaderProgram::ShaderProgram(std::string vertex_shader_path, std::string fragment_shader_path) : vertex_shader_path(std::move(vertex_shader_path)), fragment_shader_path(std::move(fragment_shader_path)) {
    if(!this->load_and_compile()) {
        throw std::runtime_error("Failed to construct Shader Program");
    }
}

ShaderProgram::~ShaderProgram() {
    if(this->gl_program_handle != 0) {
        glDeleteProgram(this->gl_program_handle);
        this->gl_program_handle = 0;
    }
}
