#include "chunk.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "chunk_mesh.hpp"
#include "glad/glad.h"
#include "shader_program.hpp"

Chunk::Chunk(glm::ivec3 position)
    : position(position)
{
    for (int ypos = 0; ypos < CHUNK_SIZE_Y; ypos++) {
        for (int zpos = 0; zpos < CHUNK_SIZE_Z; zpos++) {
            for (int xpos = 0; xpos < CHUNK_SIZE_X; xpos++) {
                if ((xpos + ypos + zpos) % 2 == 0) {
                    this->blocks[(xpos * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (ypos * CHUNK_SIZE_Z) + zpos] = 1;
                }
            }
        }
    }

    //this->blocks.fill(1);
    this->mesh = std::make_unique<ChunkMesh>();

    this->mesh->generate(*this);
}

auto Chunk::validate_coordinates(glm::ivec3 position) const noexcept -> bool
{
    if (position.x < 0 || position.x > static_cast<int>(CHUNK_SIZE_X - 1)) {
        return false;
    }

    if (position.y < 0 || position.y > static_cast<int>(CHUNK_SIZE_Y - 1)) {
        return false;
    }

    if (position.z < 0 || position.z > static_cast<int>(CHUNK_SIZE_Z - 1)) {
        return false;
    }

    return true;
}

void Chunk::render() const noexcept
{
    this->mesh->render();
}

auto Chunk::get_block_at(glm::ivec3 position) const noexcept -> std::uint8_t
{
    if (!this->validate_coordinates(position)) {
        return 0;
    }

    return this->blocks[((position.x * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (position.y * CHUNK_SIZE_Z) + position.z)];
}

void Chunk::set_block_at(glm::ivec3 position, std::uint8_t block_id) noexcept
{
    this->blocks[((position.x * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (position.y * CHUNK_SIZE_Z) + position.z)] = block_id;
}

auto Chunk::get_position() const noexcept -> glm::ivec3
{
    return this->position;
}

void Chunk::set_position(glm::ivec3 position) noexcept
{
    this->position = position;
}
