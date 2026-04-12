#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <memory>

#include "block.hpp"
#include "chunk_mesh.hpp"
#include "coordinates.hpp"

class Chunk
{

private:
    ChunkCoord position;
    bool dirty = true;

    std::unique_ptr<std::array<Block, CHUNK_SIZE>> blocks = nullptr;
    std::uint16_t air_block_count = CHUNK_SIZE;

    std::unique_ptr<ChunkMesh> mesh = std::make_unique<ChunkMesh>();

    void initialize_block_array();

public:
    Chunk(ChunkCoord position)
        : position(position) { };
    ~Chunk() = default;

    // disable copying
    Chunk(const Chunk&) = delete;
    auto operator=(const Chunk&) -> Chunk& = delete;

    // enable moving
    Chunk(Chunk&&) noexcept = default;
    auto operator=(Chunk&&) noexcept -> Chunk& = default;

    [[nodiscard]] auto get_block_at(LocalCoord position) const noexcept -> Block;
    void set_block_at(LocalCoord position, Block block, bool supress_dirty = false);

    [[nodiscard]] auto get_position() const noexcept -> ChunkCoord;

    static auto calculate_hash(const ChunkCoord& position) noexcept -> std::uint64_t;
    [[nodiscard]] auto get_hash() const noexcept -> std::uint64_t;

    [[nodiscard]] auto is_dirty() const noexcept -> bool;
    void set_dirty(bool dirty) noexcept;

    [[nodiscard]] auto is_empty() const noexcept -> bool;

    void generate_mesh(const World& world);
    void render() const noexcept;
};

