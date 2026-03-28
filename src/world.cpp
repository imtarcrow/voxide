#include "world.hpp"

#include <random>
#include <spdlog/spdlog.h>

#include "FastNoiseLite.h"
#include "chunk.hpp"
#include "coordinates.hpp"

World::World()
{
    this->noise_generator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    this->noise_generator.SetSeed(1337);
    this->noise_generator.SetFrequency(0.02F);

    this->noise_generator.SetFractalType(FastNoiseLite::FractalType_FBm);
    this->noise_generator.SetFractalOctaves(3);
    this->noise_generator.SetFractalLacunarity(2.0F);
    this->noise_generator.SetFractalGain(0.7F);
}

auto World::is_chunk_loaded(ChunkCoord position) const noexcept -> bool
{
    std::uint64_t key = Chunk::calculate_chunk_key(position);
    return this->loaded_chunks.contains(key);
}

[[nodiscard]] auto World::get_seed() const noexcept -> int
{
    return this->seed;
}

void World::set_seed(int seed) noexcept
{
    this->seed = seed;
}

[[nodiscard]] auto World::try_get_block(WorldCoord position) const noexcept -> std::optional<Block>
{
    if (!this->is_chunk_loaded(CoordConvert::world_to_chunk(position))) {
        return std::nullopt;
    }

    const auto& chunk = this->loaded_chunks.at(Chunk::calculate_chunk_key(CoordConvert::world_to_chunk(position)));
    return chunk->get_block_at(CoordConvert::world_to_local(position));
}

auto World::try_set_block(WorldCoord position, Block block) noexcept -> bool
{
    if (!this->is_chunk_loaded(CoordConvert::world_to_chunk(position))) {
        return false;
    }

    auto& chunk = this->loaded_chunks.at(Chunk::calculate_chunk_key(CoordConvert::world_to_chunk(position)));
    chunk->set_block_at(CoordConvert::world_to_local(position), block);

    return true;
}

[[nodiscard]] auto World::try_get_chunk(ChunkCoord position) -> Chunk*
{
    if (!this->is_chunk_loaded(position)) {
        return nullptr;
    }

    auto iterator = this->loaded_chunks.find(Chunk::calculate_chunk_key(position));
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
    auto [iterator, has_emplaced]
        = this->loaded_chunks.try_emplace(Chunk::calculate_chunk_key(position), std::make_unique<Chunk>(position));

    for (int xpos = 0; xpos < CHUNK_SIZE_X; xpos++) {
        for (int zpos = 0; zpos < CHUNK_SIZE_Z; zpos++) {
            float value = this->noise_generator.GetNoise(static_cast<float>(xpos + (position.x * 16)),
                                                         static_cast<float>(zpos + (position.z * 16)));
            int height = static_cast<int>(((value + 1.0F) / 2.0F) * 96);

            for (int ypos = 0; ypos < CHUNK_SIZE_Y; ypos++) {
                if (ypos + (position.y * static_cast<int>(CHUNK_SIZE_Y)) < height - 1)
                    iterator->second->set_block_at(LocalCoord(xpos, ypos, zpos), Block::Stone);
                else if (ypos + (position.y * static_cast<int>(CHUNK_SIZE_Y)) < height)
                    iterator->second->set_block_at(LocalCoord(xpos, ypos, zpos), Block::Grass);
            }
        }
    }

    std::random_device dev;
    std::mt19937_64 rng(this->seed + Chunk::calculate_chunk_key(position));
    std::uniform_real_distribution<double> dist(0, 1);

    for (int xpos = 0; xpos < CHUNK_SIZE_X; xpos++) {
        for (int ypos = 0; ypos < CHUNK_SIZE_X; ypos++) {
            for (int zpos = 0; zpos < CHUNK_SIZE_X; zpos++) {
                if (dist(rng) < 0.999) {
                    continue;
                }

                iterator->second->set_block_at(LocalCoord(xpos, ypos, zpos), Block::Blue);
            }
        }
    }

    return *iterator->second;
}

void World::generate_area(ChunkCoord center, int x_radius, int y_radius, int z_radius)
{
    for (int xpos = -(x_radius / 2); xpos < (x_radius / 2); xpos++) {
        for (int zpos = -(z_radius / 2); zpos < (z_radius / 2); zpos++) {
            for (int ypos = -(y_radius / 2); ypos < (y_radius / 2); ypos++) {
                this->generate_chunk(center + ChunkCoord(xpos, ypos, zpos));
            }
        }
    }

    spdlog::info("generating meshes");
    spdlog::info("loaded chunks size: {}", this->loaded_chunks.size());

    for (int xpos = -(x_radius / 2); xpos < (x_radius / 2); xpos++) {
        for (int zpos = -(z_radius / 2); zpos < (z_radius / 2); zpos++) {
            for (int ypos = -(y_radius / 2); ypos < (y_radius / 2); ypos++) {
                auto* chunk = this->try_get_chunk(ChunkCoord(xpos, ypos, zpos));

                if (chunk == nullptr)
                    continue;

                chunk->generate_mesh(*this);
            }
        }
    }
}
