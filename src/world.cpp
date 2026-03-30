#include "world.hpp"

#include <cstdint>
#include <spdlog/spdlog.h>

#include "FastNoiseLite.h"
#include "chunk.hpp"
#include "coordinates.hpp"

World::World()
{
    this->noise_generator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    this->noise_generator.SetSeed(this->seed);
    this->noise_generator.SetFrequency(0.02F);

    this->noise_generator.SetFractalType(FastNoiseLite::FractalType_FBm);
    this->noise_generator.SetFractalOctaves(3);
    this->noise_generator.SetFractalLacunarity(2.0F);
    this->noise_generator.SetFractalGain(0.7F);
}

auto World::is_chunk_loaded(ChunkCoord position) const noexcept -> bool
{
    std::uint64_t key = Chunk::calculate_hash(position);
    return this->loaded_chunks.contains(key);
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

[[nodiscard]] auto World::try_get_block(WorldCoord position) const noexcept -> std::optional<Block>
{
    if (!this->is_chunk_loaded(Coords::world_to_chunk(position))) {
        return std::nullopt;
    }

    const auto& chunk = this->loaded_chunks.at(Chunk::calculate_hash(Coords::world_to_chunk(position)));
    return chunk->get_block_at(Coords::world_to_local(position));
}

auto World::try_set_block(WorldCoord position, Block block) noexcept -> bool
{
    if (!this->is_chunk_loaded(Coords::world_to_chunk(position))) {
        return false;
    }

    auto& chunk = this->loaded_chunks.at(Chunk::calculate_hash(Coords::world_to_chunk(position)));
    chunk->set_block_at(Coords::world_to_local(position), block);

    return true;
}

[[nodiscard]] auto World::try_get_chunk(ChunkCoord position) const -> Chunk*
{
    if (!this->is_chunk_loaded(position)) {
        return nullptr;
    }

    auto iterator = this->loaded_chunks.find(Chunk::calculate_hash(position));
    if (iterator == this->loaded_chunks.end()) {
        return nullptr;
    }

    return iterator->second.get();
}

auto World::get_loaded_chunks() const -> const std::unordered_map<uint64_t, std::unique_ptr<Chunk>>&
{
    return this->loaded_chunks;
}

auto World::generate_chunk(ChunkCoord position) -> Chunk&
{

    std::uint64_t hash = Chunk::calculate_hash(position);

    auto [iterator, has_emplaced]
        = this->loaded_chunks.try_emplace(hash, std::make_unique<Chunk>(position));

    for (int xpos = 0; xpos < CHUNK_SIZE_X; xpos++) {
        for (int zpos = 0; zpos < CHUNK_SIZE_Z; zpos++) {
            float value = this->noise_generator.GetNoise(static_cast<float>(xpos + (position.x * CHUNK_SIZE_X)),
                                                         static_cast<float>(zpos + (position.z * CHUNK_SIZE_Z)));
            int height = static_cast<int>(((value + 1.0F) / 2.0F) * 64);

            for (int ypos = 0; ypos < CHUNK_SIZE_Y; ypos++) {
                if (ypos + (position.y * static_cast<int>(CHUNK_SIZE_Y)) < 32) {
                    iterator->second->set_block_at(LocalCoord(xpos, ypos, zpos), Block::Water);
                }
                if (ypos + (position.y * static_cast<int>(CHUNK_SIZE_Y)) < height - 1)
                    iterator->second->set_block_at(LocalCoord(xpos, ypos, zpos), Block::Stone);
                else if (ypos + (position.y * static_cast<int>(CHUNK_SIZE_Y)) < height)
                    iterator->second->set_block_at(LocalCoord(xpos, ypos, zpos), Block::Grass);
            }
        }
    }

    return *iterator->second;
}

void World::generate_area(ChunkCoord center, int x_radius, int y_radius, int z_radius)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (int xpos = -(x_radius / 2); xpos < (x_radius / 2); xpos++) {
        for (int zpos = -(z_radius / 2); zpos < (z_radius / 2); zpos++) {
            for (int ypos = -1; ypos < y_radius; ypos++) {
                this->generate_chunk(center + ChunkCoord(xpos, ypos, zpos));
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    spdlog::info("Generating chunks took {}ms", duration.count());

    start = std::chrono::high_resolution_clock::now();
}
