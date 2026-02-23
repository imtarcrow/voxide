#pragma once
#ifndef VOXIDE_SHADER_PROGRAM_HEADER

#include <glm/glm.hpp>
#include <optional>
#include <string>

#include "glad/glad.h"

class ShaderProgram
{
private:
    GLuint gl_program_handle = 0;

    [[nodiscard]] static auto create_shader(const std::string& filepath, int shader_type) -> std::optional<GLuint>;

    static auto load_shader_file(const std::string& path) -> std::optional<std::string>;
    auto load_and_compile() -> bool;

    std::string vertex_shader_path;
    std::string fragment_shader_path;

public:
    ShaderProgram(std::string vertex_shader_path, std::string fragment_shader_path);
    ~ShaderProgram();

    // disable copying
    ShaderProgram(const ShaderProgram&) = delete;
    auto operator=(const ShaderProgram&) -> ShaderProgram& = delete;

    // Enable cheap moving
    ShaderProgram(ShaderProgram&&) noexcept = default;
    auto operator=(ShaderProgram&&) noexcept -> ShaderProgram& = default; // enable moving

    [[nodiscard]] auto use() const -> bool;
    auto reload() -> bool;
    void set_uniform(const std::string& name, glm::mat4 value) const;
};

#endif // !VOXIDE_SHADER_PROGRAM_HEADER
