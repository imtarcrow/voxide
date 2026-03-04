#pragma once
#include <glm/fwd.hpp>
#ifndef VOXIDE_CHUNK_HEADER
#define VOXIDE_CHUNK_HEADER

#include <cstddef>
#include <glm/glm.hpp>

#include "glad/glad.h"

class Chunk
{

private:
    glm::ivec3 position;

    std::array<char, static_cast<std::size_t>(16 * 16 * 16)> blocks = { 0 };

    auto is_in_chunk(glm::ivec3 position) -> bool;

public:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    int indicies_size = 0;

    Chunk(glm::ivec3 position);
    ~Chunk();

    // disable copying
    Chunk(const Chunk&) = delete;
    auto operator=(const Chunk&) -> Chunk& = delete;

    // enable moving
    Chunk(Chunk&&) noexcept = default;
    auto operator=(Chunk&&) noexcept -> Chunk& = default;

    void gen_chunk_mesh();

    auto get_block_at(glm::ivec3 position) -> char;
    void set_block_at(glm::ivec3 position, char block_id);
};

#endif // VOXIDE_CHUNK_HEADER
