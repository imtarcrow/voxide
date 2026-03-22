#include "chunk_mesh.hpp"

#include <cstdint>
#include <glm/fwd.hpp>
#include <spdlog/spdlog.h>

#include "chunk.hpp"
#include "coordinates.hpp"
#include "glad/glad.h"
#include "world.hpp"

ChunkMesh::ChunkMesh()
{
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glGenVertexArrays(1, &this->VAO);

    if (this->VBO == 0 || this->EBO == 0) {
        GLenum error = glGetError();
        spdlog::error("glGenBuffers returned an Error: {} ", error);
        throw std::runtime_error("glGenBuffers returned an Error");
    }
    if (this->VAO == 0) {
        GLenum error = glGetError();
        spdlog::error("glGenVertexArrays returned an Error: {}", error);
        throw std::runtime_error("glGenVertexArrays returned an Error");
    }
}

ChunkMesh::~ChunkMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

ChunkMesh::ChunkMesh(ChunkMesh&& other) noexcept
    : VAO(other.VAO)
    , VBO(other.VBO)
    , EBO(other.EBO)
    , index_count(other.index_count)
{
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.index_count = 0;
}

auto ChunkMesh::operator=(ChunkMesh&& other) noexcept -> ChunkMesh&
{
    if (this == &other) {
        return *this;
    }

    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
    if (EBO)
        glDeleteBuffers(1, &EBO);

    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
    index_count = other.index_count;

    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.index_count = 0;

    return *this;
}

auto ChunkMesh::pack_vertex_data(VertexData data) const noexcept -> std::uint32_t
{
    std::uint32_t packed = 0;

    packed |= static_cast<std::uint32_t>(data.position.x & 0x1FU) << 27U;
    packed |= static_cast<std::uint32_t>(data.position.y & 0x1FU) << 22U;
    packed |= static_cast<std::uint32_t>(data.position.z & 0x1FU) << 17U;
    packed |= static_cast<std::uint32_t>(data.texture & 0xFFU) << 9U;
    packed |= static_cast<std::uint32_t>(data.direction & 0x7U) << 6U;
    packed |= static_cast<std::uint32_t>(data.corner & 0x3U) << 4U;
    packed |= static_cast<std::uint32_t>(data.ambient_occlusion & 0x3U) << 2U;

    return packed;
}

void ChunkMesh::generate(const Chunk& chunk, const World& world)
{
    std::vector<std::uint32_t> packed_vertex_data;
    std::vector<GLuint> indicies;

    auto get_block = [&](int xpos, int ypos, int zpos) -> Block {
        LocalCoord local(xpos, ypos, zpos);
        if (Chunk::is_inside_chunk(local))
            return chunk.get_block_at(local).value();
        auto block = world.try_get_block(CoordConvert::local_to_world(local, chunk.get_position()));
        return block.value_or(Block::Air);
    };

    auto push_face = [&](glm::uvec3 pos, Direction direction, Block block) -> void {
        GLuint base = packed_vertex_data.size();
        indicies.insert(indicies.end(), { base + 0, base + 1, base + 2, base + 0, base + 2, base + 3 });
        packed_vertex_data.push_back(
            this->pack_vertex_data({ pos + corner_positions[direction][0], static_cast<std::uint8_t>(block), direction, 0, 0 }));
        packed_vertex_data.push_back(
            this->pack_vertex_data({ pos + corner_positions[direction][1], static_cast<std::uint8_t>(block), direction, 1, 0 }));
        packed_vertex_data.push_back(
            this->pack_vertex_data({ pos + corner_positions[direction][2], static_cast<std::uint8_t>(block), direction, 2, 0 }));
        packed_vertex_data.push_back(
            this->pack_vertex_data({ pos + corner_positions[direction][3], static_cast<std::uint8_t>(block), direction, 3, 0 }));
    };

    for (int ypos = 0; ypos < CHUNK_SIZE_Y; ypos++) {
        for (int zpos = 0; zpos < CHUNK_SIZE_Z; zpos++) {
            for (int xpos = 0; xpos < CHUNK_SIZE_X; xpos++) {

                Block current_block = chunk.get_block_at(LocalCoord(xpos, ypos, zpos)).value();
                if (current_block == Block::Air) {
                    continue;
                }

                // X+ facing
                Block neighbor = get_block(xpos + 1, ypos, zpos);
                if (neighbor == Block::Air) {
                    push_face(glm::uvec3(xpos, ypos, zpos), Direction::XPos, current_block);
                }

                // X- facing
                neighbor = get_block(xpos - 1, ypos, zpos);
                if (neighbor == Block::Air) {
                    push_face(glm::uvec3(xpos, ypos, zpos), Direction::XNeg, current_block);
                }

                // Y+ facing
                neighbor = get_block(xpos, ypos + 1, zpos);
                if (neighbor == Block::Air) {
                    push_face(glm::uvec3(xpos, ypos, zpos), Direction::YPos, current_block);
                }

                // Y- facing
                neighbor = get_block(xpos, ypos - 1, zpos);
                if (neighbor == Block::Air) {
                    push_face(glm::uvec3(xpos, ypos, zpos), Direction::YNeg, current_block);
                }

                // Z+ facing
                neighbor = get_block(xpos, ypos, zpos + 1);
                if (neighbor == Block::Air) {
                    push_face(glm::uvec3(xpos, ypos, zpos), Direction::ZPos, current_block);
                }

                // Z- facing
                neighbor = get_block(xpos, ypos, zpos - 1);
                if (neighbor == Block::Air) {
                    push_face(glm::uvec3(xpos, ypos, zpos), Direction::ZNeg, current_block);
                }
            }
        }
    }

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    auto verticies_size = static_cast<GLsizei>(packed_vertex_data.size() * sizeof(std::uint32_t));
    auto indicies_size = static_cast<GLsizei>(indicies.size() * sizeof(std::uint32_t));

    glBufferData(GL_ARRAY_BUFFER, verticies_size, packed_vertex_data.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies_size, indicies.data(), GL_DYNAMIC_DRAW);

    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(std::uint32_t), nullptr);
    glEnableVertexAttribArray(0);

    this->index_count = static_cast<GLsizei>(indicies.size());
    ChunkCoord chunk_position = chunk.get_position();
    spdlog::debug("Generated Chunk mesh at position x={},y={},z={} \n - Vertex data: {}B, Index data: {}B", chunk_position.x,
                  chunk_position.y, chunk_position.z, packed_vertex_data.size() * sizeof(std::uint32_t), indicies.size() * sizeof(GLuint));
}

void ChunkMesh::render() const noexcept
{
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->index_count, GL_UNSIGNED_INT, nullptr);
}
