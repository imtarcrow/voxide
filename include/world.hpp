#pragma once

#include <cstdint>
#include <noise/FastNoiseLite.h>
#include <optional>
#include <unordered_map>

#include "chunk.hpp"
#include "coordinates.hpp"

class World
{
private:
    int seed = 0;
    FastNoiseLite noise_generator;

    std::unordered_map<std::uint64_t, std::unique_ptr<Chunk>> loaded_chunks;

    [[nodiscard]] auto is_chunk_loaded(ChunkCoord position) const noexcept -> bool;

public:
    World();
    ~World() = default;

    // disable copying
    World(const World&) = delete;
    auto operator=(const World&) -> World& = delete;

    // enable moving
    World(World&&) noexcept = default;
    auto operator=(World&&) noexcept -> World& = default;

    [[nodiscard]] auto try_get_block(WorldCoord position) const noexcept -> std::optional<Block>;
    // [[nodiscard]] auto get_block(WorldCoord position) -> Block;

    auto try_set_block(WorldCoord position, Block block) noexcept -> bool;
    // void set_block_at(WorldCoord position, Block block);

    [[nodiscard]] auto try_get_chunk(ChunkCoord position) const -> Chunk*;
    // [[nodiscard]] auto get_chunk(ChunkCoord position) -> Chunk&;

    [[nodiscard]] auto get_loaded_chunks() const -> const std::unordered_map<uint64_t, std::unique_ptr<Chunk>>&;

    [[nodiscard]] auto get_seed() const noexcept -> int;
    void set_seed(int seed) noexcept;

    auto generate_chunk(ChunkCoord position) -> Chunk&;
    void generate_area(ChunkCoord center, int x_radius, int y_radius, int z_radius);
};
