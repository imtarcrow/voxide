#include "chunk.hpp"

#include <FastNoiseLite.h>
#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "chunk_mesh.hpp"

Chunk::Chunk(ChunkCoord position)
    : position(position)
{
    this->mesh = std::make_unique<ChunkMesh>();
}

auto Chunk::is_inside_chunk(LocalCoord position) noexcept -> bool
{
    return position.x >= 0 && position.x < static_cast<int>(CHUNK_SIZE_X) && position.y >= 0 && position.y < static_cast<int>(CHUNK_SIZE_Y)
        && position.z >= 0 && position.z < static_cast<int>(CHUNK_SIZE_Z);
}

auto Chunk::get_block_at(LocalCoord position) const noexcept -> std::optional<Block>
{
    if (!Chunk::is_inside_chunk(position)) {
        return std::nullopt;
    }

    return this->blocks[(position.x * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (position.y * CHUNK_SIZE_Z) + position.z];
}

void Chunk::set_block_at(LocalCoord position, Block block) noexcept
{
    this->blocks[(position.x * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (position.y * CHUNK_SIZE_Z) + position.z] = block;
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

void Chunk::generate_mesh(const World& world)
{
    this->mesh->generate(*this, world);
}

void Chunk::render() const noexcept
{
    this->mesh->render();
}
