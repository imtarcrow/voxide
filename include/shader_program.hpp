#pragma once
#ifndef VOXIDE_SHADER_PROGRAM_HEADER

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <string>

class ShaderProgram {
   private:
    GLuint program_id;

    static auto createShader(const std::string &path, int type) -> GLuint;
    static auto readShaderFile(const std::string &path) -> std::string;
    auto loadAndCompile() -> bool;

    std::string vertex_shader_path;
    std::string fragment_shader_path;

   public:
    ShaderProgram(const std::string &vertex_shader_path, const std::string &fragment_shader_path);
    ~ShaderProgram();

    // disable copying
    ShaderProgram(const ShaderProgram &) = delete;
    auto operator=(const ShaderProgram &) -> ShaderProgram & = delete;

 // Enable cheap moving
    ShaderProgram(ShaderProgram&&) noexcept            = default;
    auto operator=(ShaderProgram&&) noexcept -> ShaderProgram& = default;   // enable moving

    [[nodiscard]] auto use() const -> bool;
    [[nodiscard]] auto reload() -> bool;
    void setUniform(const std::string &name, glm::mat4 value) const;
};


#endif // !VOXIDE_SHADER_PROGRAM_HEADER
