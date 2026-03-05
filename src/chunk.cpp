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
    for (int y = 0; y < CHUNK_SIZE_Y; y++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            for (int x = 0; x < CHUNK_SIZE_X; x++) {
                if ((x + y + z) % 2 == 0) {
                    this->blocks[(x * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (y * CHUNK_SIZE_Z) + z] = 1;
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

void Chunk::render(ShaderProgram& program) const noexcept
{
    if (!program.use()) {
        spdlog::error("Failed to use Shader Program");
    }

    auto model = glm::mat4(1.0F);
    model = glm::translate(model, { this->position.x * CHUNK_SIZE_X, this->position.y * CHUNK_SIZE_Y, this->position.z * CHUNK_SIZE_Z });


    program.set_uniform("model", model);
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
