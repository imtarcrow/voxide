#include "chunk.hpp"

#include <FastNoiseLite.h>
#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "block.hpp"
#include "chunk_mesh.hpp"

Chunk::Chunk(ChunkCoord position)
    : position(position)
{
    this->mesh = std::make_unique<ChunkMesh>();
}

void Chunk::initialize_block_array()
{
    if (!this->is_empty()) {
        return;
    }

    this->blocks = std::make_unique<std::array<Block, CHUNK_SIZE>>();
    this->blocks->fill(Block::Air);
}

auto Chunk::get_block_at(LocalCoord position) const noexcept -> Block
{
    if (this->is_empty()) {
        return Block::Air;
    }

    return (*this->blocks)[(position.x * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (position.y * CHUNK_SIZE_Z) + position.z];
}

void Chunk::set_block_at(LocalCoord position, Block block)
{
    if (this->is_empty() && block == Block::Air) {
        return;
    }

    if (this->is_empty()) {
        this->initialize_block_array();
    }

    const uint16_t index = (position.x * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (position.y * CHUNK_SIZE_Z) + position.z;
    Block current_block = (*this->blocks)[index];

    if (current_block != block) {
        (*this->blocks)[index] = block;

        if (current_block == Block::Air && block != Block::Air) {
            this->air_block_count--;
        }
        else if (current_block != Block::Air && block == Block::Air) {
            this->air_block_count++;
        }

        // delete chunk array if chunk is completly made of air
        if (air_block_count == CHUNK_SIZE) {
            this->blocks = nullptr;
        }
    }
}

auto Chunk::get_position() const noexcept -> ChunkCoord
{
    return this->position;
}

void Chunk::set_position(ChunkCoord position) noexcept
{
    this->position = position;
}

auto Chunk::calculate_chunk_key(const ChunkCoord& position) noexcept -> uint64_t
{
    uint64_t hash = 0;

    hash ^= static_cast<uint64_t>(position.x) * 2654435761;
    hash ^= static_cast<uint64_t>(position.y) * 2654435761 << 21U;
    hash ^= static_cast<uint64_t>(position.z) * 2654435761 << 42U;
    hash ^= (hash >> 30U);
    hash *= 0xbf58476d1ce4e5b;
    hash ^= (hash >> 27U);
    hash *= 0x94d049bb133111eb;
    hash ^= (hash >> 31U);

    return hash;
}

auto Chunk::get_chunk_key() const noexcept -> std::uint64_t
{
    return Chunk::calculate_chunk_key(this->position);
}

auto Chunk::is_empty() const noexcept -> bool
{
    return this->blocks == nullptr;
}

void Chunk::generate_mesh(const World& world)
{
    this->mesh->generate(*this, world);
}

void Chunk::render() const noexcept
{
    this->mesh->render();
}
