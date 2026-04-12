#include "chunk.hpp"

#include <cstdint>
#include <noise/FastNoiseLite.h>
#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "block.hpp"
#include "chunk_mesh.hpp"

void Chunk::initialize_block_array()
{
    if (!this->is_empty()) {
        return;
    }

    this->blocks = std::make_unique<std::array<Block, CHUNK_SIZE>>();
    this->blocks->fill(Block::AIR);
}

auto Chunk::get_block_at(LocalCoord position) const noexcept -> Block
{
    if (this->is_empty()) {
        return Block::AIR;
    }

    return (*this->blocks)[position.to_index()];
}

void Chunk::set_block_at(LocalCoord position, Block block, bool supress_dirty)
{
    if (this->is_empty() && block == Block::AIR) {
        return;
    }

    if (this->is_empty()) {
        this->initialize_block_array();
    }

    const std::uint16_t index = position.to_index();
    Block current_block = (*this->blocks)[index];

    if (current_block != block) {
        (*this->blocks)[index] = block;

        if(block != Block::AIR && current_block == Block::AIR) {
            this->air_block_count--;
        } else if (block == Block::AIR && current_block != Block::AIR) {
            this->air_block_count++;
        }

        // delete chunk array if chunk is completly made of air
        if (air_block_count == CHUNK_SIZE) {
            this->blocks = nullptr;
        }
    }

    if (!supress_dirty) {
        this->dirty = true;
    }
}

auto Chunk::get_position() const noexcept -> ChunkCoord
{
    return this->position;
}

auto Chunk::calculate_hash(const ChunkCoord& position) noexcept -> uint64_t
{
    auto xpos = static_cast<uint64_t>(static_cast<uint32_t>(position.x));
    auto ypos = static_cast<uint64_t>(static_cast<uint32_t>(position.y));
    auto zpos = static_cast<uint64_t>(static_cast<uint32_t>(position.z));

    uint64_t hash = 0;

    hash ^= xpos * 0x9E3779B185EBCA87ULL;
    hash ^= ypos * 0xC2B2AE3D27D4EB4FULL;
    hash ^= zpos * 0x165667B19E3779F9ULL;

    hash ^= (hash >> 30U);
    hash *= 0xbf58476d1ce4e5bULL;
    hash ^= (hash >> 27U);
    hash *= 0x94d049bb133111ebULL;
    hash ^= (hash >> 31U);

    return hash;
}

auto Chunk::get_hash() const noexcept -> std::uint64_t
{
    return Chunk::calculate_hash(this->position);
}

auto Chunk::is_empty() const noexcept -> bool
{
    return this->blocks == nullptr;
}

auto Chunk::is_dirty() const noexcept -> bool
{
    return this->dirty;
}

void Chunk::set_dirty(bool dirty) noexcept
{
    this->dirty = dirty;
}

void Chunk::generate_mesh(const World& world)
{
    this->mesh->generate(*this, world);
}

void Chunk::render() const noexcept
{
    this->mesh->render();
}
