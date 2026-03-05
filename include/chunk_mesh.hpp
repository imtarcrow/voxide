#pragma once
#ifndef VOXIDE_CHUNK_MESH_HEADER
#define VOXIDE_CHUNK_MESH_HEADER

#include "glad/glad.h"
#include <glm/glm.hpp>

using Vertex = struct Vertex {
    glm::uvec3 position{0.0F, 0.0F, 0.0F};
    std::uint8_t dir;
    unsigned int texture{};
};

class Chunk;

class ChunkMesh
{
private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    unsigned long indicies_size = 0;

    [[nodiscard]] auto pack_vertex_data(glm::uvec3 position, std::uint8_t direction, std::uint8_t texture) const noexcept -> std::uint32_t;

public: 
    ChunkMesh();
    ~ChunkMesh();

    // disable copying
    ChunkMesh(const ChunkMesh&) = delete;
    auto operator=(const ChunkMesh&) -> ChunkMesh& = delete;

    // enable moving
    ChunkMesh(ChunkMesh&&) noexcept = default;
    auto operator=(ChunkMesh&&) noexcept -> ChunkMesh& = default;

    void generate(const Chunk& chunk);
    void render() const;
};

#endif // VOXIDE_CHUNK_MESH_HEADER
