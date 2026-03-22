#pragma once
#ifndef VOXIDE_CHUNK_MESH_HEADER
#define VOXIDE_CHUNK_MESH_HEADER

#include <glm/glm.hpp>
#include "glad/glad.h"

class Chunk;
class World;

enum Direction : std::uint8_t {
    XPos = 0,
    XNeg = 1,
    YPos = 2,
    YNeg = 3,
    ZPos = 4,
    ZNeg = 5,
};

using VertexData = struct VertexData
{
    glm::uvec3 position;
    std::uint16_t texture;
    Direction direction;
    std::uint8_t corner;
    std::uint8_t ambient_occlusion;
};

class ChunkMesh
{
private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLsizei index_count = 0;

    [[nodiscard]] auto pack_vertex_data(VertexData data) const noexcept -> std::uint32_t;

public:
    ChunkMesh();
    ~ChunkMesh();

    // disable copying
    ChunkMesh(const ChunkMesh&) = delete;
    auto operator=(const ChunkMesh&) -> ChunkMesh& = delete;

    // enable moving
    ChunkMesh(ChunkMesh&&) noexcept;
    auto operator=(ChunkMesh&&) noexcept -> ChunkMesh&;

    void generate(const Chunk& chunk, const World& world);
    void render() const noexcept;
};

#endif // VOXIDE_CHUNK_MESH_HEADER
