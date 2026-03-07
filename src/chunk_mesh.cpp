#include "chunk_mesh.hpp"

#include <cstdint>
#include <glm/fwd.hpp>
#include <spdlog/spdlog.h>

#include "chunk.hpp"
#include "glad/glad.h"

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

auto ChunkMesh::pack_vertex_data(glm::uvec3 position, std::uint8_t direction, std::uint8_t texture) const noexcept -> std::uint32_t
{
    std::uint32_t data = 0;

    data = data | (static_cast<std::uint32_t>(position.x) << 0U);
    data = data | (static_cast<std::uint32_t>(position.y) << 5U);
    data = data | (static_cast<std::uint32_t>(position.z) << 10U);
    data = data | (static_cast<std::uint32_t>(direction) << 15U);
    data = data | ((static_cast<std::uint32_t>(texture) & 0x3FU) << 18U);

    return data;
}

void ChunkMesh::generate(const Chunk& chunk)
{
    std::vector<std::uint32_t> verticies;
    std::vector<GLuint> indicies;

    auto push_face = [&](unsigned int xpos, unsigned int ypos, unsigned int zpos, glm::uvec3 pos0, glm::uvec3 pos1, glm::uvec3 pos2,
                         glm::uvec3 pos3, std::uint8_t dir, std::uint8_t texture) -> void {
        GLuint base = verticies.size();
        indicies.insert(indicies.end(), { base + 0, base + 1, base + 2, base + 0, base + 2, base + 3 });
        verticies.push_back(this->pack_vertex_data(glm::uvec3(xpos, ypos, zpos) + pos0, dir, texture));
        verticies.push_back(this->pack_vertex_data(glm::uvec3(xpos, ypos, zpos) + pos1, dir, texture));
        verticies.push_back(this->pack_vertex_data(glm::uvec3(xpos, ypos, zpos) + pos2, dir, texture));
        verticies.push_back(this->pack_vertex_data(glm::uvec3(xpos, ypos, zpos) + pos3, dir, texture));
    };

    for (unsigned int ypos = 0; ypos < CHUNK_SIZE_Y; ypos++) {
        for (unsigned int zpos = 0; zpos < CHUNK_SIZE_Z; zpos++) {
            for (unsigned int xpos = 0; xpos < CHUNK_SIZE_X; xpos++) {

                std::uint8_t current_block = chunk.get_block_at(glm::ivec3(xpos, ypos, zpos));
                if (current_block == 0) {
                    continue;
                }

                // X+ facing
                std::uint8_t block_id = chunk.get_block_at({ xpos + 1, ypos, zpos });
                if (block_id == 0) {
                    push_face(xpos, ypos, zpos, { 1, 1, 1 }, { 1, 0, 1 }, { 1, 0, 0 }, { 1, 1, 0 }, 0, current_block);
                }

                // X- facing
                block_id = chunk.get_block_at({ xpos - 1, ypos, zpos });
                if (block_id == 0) {
                    push_face(xpos, ypos, zpos, { 0, 1, 0 }, { 0, 0, 0 }, { 0, 0, 1 }, { 0, 1, 1 }, 1, current_block);
                }

                // Z+ facing
                block_id = chunk.get_block_at({ xpos, ypos, zpos + 1 });
                if (block_id == 0) {
                    push_face(xpos, ypos, zpos, { 0, 1, 1 }, { 0, 0, 1 }, { 1, 0, 1 }, { 1, 1, 1 }, 2, current_block);
                }

                // Z- facing
                block_id = chunk.get_block_at({ xpos, ypos, zpos - 1 });
                if (block_id == 0) {
                    push_face(xpos, ypos, zpos, { 1, 1, 0 }, { 1, 0, 0 }, { 0, 0, 0 }, { 0, 1, 0 }, 3, current_block);
                }

                // Y+ facing
                block_id = chunk.get_block_at({ xpos, ypos + 1, zpos });
                if (block_id == 0) {
                    push_face(xpos, ypos, zpos, { 0, 1, 1 }, { 1, 1, 1 }, { 1, 1, 0 }, { 0, 1, 0 }, 4, current_block);
                }

                // Y- facing
                block_id = chunk.get_block_at({ xpos, ypos - 1, zpos });
                if (block_id == 0) {
                    push_face(xpos, ypos, zpos, { 1, 0, 1 }, { 0, 0, 1 }, { 0, 0, 0 }, { 1, 0, 0 }, 5, current_block);
                }
            }
        }
    }

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    auto verticies_size = static_cast<GLsizei>(verticies.size() * sizeof(std::uint32_t));
    auto indicies_size = static_cast<GLsizei>(indicies.size() * sizeof(std::uint32_t));

    glBufferData(GL_ARRAY_BUFFER, verticies_size, verticies.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies_size, indicies.data(), GL_STATIC_DRAW);

    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(std::uint32_t), nullptr);
    glEnableVertexAttribArray(0);

    this->index_count = static_cast<GLsizei>(indicies.size());

    glm::vec3 chunk_position = chunk.get_position();
    spdlog::debug("Generated Chunk mesh at position x={},y={},z={} \n - Vertex data: {}B, Index data: {}B", chunk_position.x,
                  chunk_position.y, chunk_position.z, verticies.size() * sizeof(std::uint32_t), indicies.size() * sizeof(GLuint));
}

void ChunkMesh::render() const noexcept
{
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->index_count, GL_UNSIGNED_INT, nullptr);
}
