#include "world.hpp"

#include <cstdint>
#include <spdlog/spdlog.h>

#include "chunk.hpp"
#include "coordinates.hpp"

World::World()
{
    this->noise_generator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    this->noise_generator.SetFrequency(0.02F);
    this->noise_generator.SetFractalType(FastNoiseLite::FractalType_FBm);
    this->noise_generator.SetFractalOctaves(3);
    this->noise_generator.SetFractalLacunarity(2.0F);
    this->noise_generator.SetFractalGain(0.7F);

    this->noise_generator.SetSeed(this->seed);
}

auto World::is_chunk_loaded(ChunkCoord position) const noexcept -> bool
{
    std::uint64_t hash = Chunk::calculate_hash(position);
    return this->loaded_chunks.contains(hash);
}

void World::tick()
{
    for (const auto& entry : this->loaded_chunks) {
        if (entry.second->is_dirty()) {
            events.emplace(WorldEvent { WorldEventType::ChunkDirty, entry.first, entry.second->get_position() });
            entry.second->set_dirty(false);
        }
    }
}


[[nodiscard]] auto World::try_get_block(WorldCoord position) const noexcept -> std::optional<Block>
{
    ChunkCoord chunk_position = Coords::world_to_chunk(position);
    if (!this->is_chunk_loaded(chunk_position)) {
        return std::nullopt;
    }

    std::uint64_t hash = Chunk::calculate_hash(Coords::world_to_chunk(position));
    const auto& chunk = this->loaded_chunks.at(hash);

    LocalCoord local_position = Coords::world_to_local(position);
    return chunk->get_block_at(local_position);
}

auto World::try_set_block(WorldCoord position, Block block) noexcept -> bool
{
    ChunkCoord chunk_position = Coords::world_to_chunk(position);
    if (!this->is_chunk_loaded(chunk_position)) {
        return false;
    }

    std::uint64_t hash = Chunk::calculate_hash(Coords::world_to_chunk(position));
    auto& chunk = this->loaded_chunks.at(hash);

    LocalCoord local_position = Coords::world_to_local(position);
    chunk->set_block_at(local_position, block);

    return true;
}

/* non owning pointer is being returned, should be discarded right after use */
[[nodiscard]] auto World::try_get_chunk(ChunkCoord position) const -> Chunk*
{
    if (!this->is_chunk_loaded(position)) {
        return nullptr;
    }

    std::uint64_t hash = Chunk::calculate_hash(position);
    auto iterator = this->loaded_chunks.find(hash);

    if (iterator == this->loaded_chunks.end()) {
        return nullptr;
    }

    return iterator->second.get();
}

[[nodiscard]] auto World::get_seed() const noexcept -> int
{
    return this->seed;
}

void World::set_seed(int seed) noexcept
{
    this->seed = seed;
    this->noise_generator.SetSeed(this->seed);
}

auto World::get_events() -> std::queue<WorldEvent>& {
    return this->events;
}

auto World::generate_chunk(ChunkCoord position) -> Chunk&
{
    std::uint64_t hash = Chunk::calculate_hash(position);

    auto [iterator, has_emplaced] = this->loaded_chunks.try_emplace(hash, std::make_unique<Chunk>(position));

    for (int xpos = 0; xpos < CHUNK_SIZE_X; xpos++) {
        for (int zpos = 0; zpos < CHUNK_SIZE_Z; zpos++) {

            float value = this->noise_generator.GetNoise(static_cast<float>(xpos + (position.x * CHUNK_SIZE_X)),
                                                         static_cast<float>(zpos + (position.z * CHUNK_SIZE_Z)));
            int height = static_cast<int>(((value + 1.0F) / 2.0F) * 64);

            for (int ypos = 0; ypos < CHUNK_SIZE_Y; ypos++) {
                if (ypos + (position.y * static_cast<int>(CHUNK_SIZE_Y)) < 32) {
                    iterator->second->set_block_at(LocalCoord(xpos, ypos, zpos), Block::WATER);
                }
                if (ypos + (position.y * static_cast<int>(CHUNK_SIZE_Y)) < height - 1)
                    iterator->second->set_block_at(LocalCoord(xpos, ypos, zpos), Block::STONE);
                else if (ypos + (position.y * static_cast<int>(CHUNK_SIZE_Y)) < height)
                    iterator->second->set_block_at(LocalCoord(xpos, ypos, zpos), Block::GRASS);
            }
        }
    }

    this->events.emplace(WorldEvent { WorldEventType::ChunkLoaded, hash, position});

    return *iterator->second;
}

void World::generate_area(ChunkCoord center, int x_radius, int y_radius, int z_radius)
{
    for (int xpos = -(x_radius / 2); xpos < (x_radius / 2); xpos++) {
        for (int zpos = -(z_radius / 2); zpos < (z_radius / 2); zpos++) {
            for (int ypos = -1; ypos < y_radius; ypos++) {
                this->generate_chunk(center + ChunkCoord(xpos, ypos, zpos));
            }
        }
    }
}
