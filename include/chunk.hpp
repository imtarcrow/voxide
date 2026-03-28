#pragma once
#ifndef VOXIDE_CHUNK_HEADER
#define VOXIDE_CHUNK_HEADER

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

    std::unique_ptr<std::array<Block, CHUNK_SIZE>> blocks = nullptr;
    uint16_t air_block_count = CHUNK_SIZE;

    std::unique_ptr<ChunkMesh> mesh;

    void initialize_block_array();

public:
    Chunk(ChunkCoord position);
    ~Chunk() = default;

    // disable copying
    Chunk(const Chunk&) = delete;
    auto operator=(const Chunk&) -> Chunk& = delete;

    // enable moving
    Chunk(Chunk&&) noexcept = default;
    auto operator=(Chunk&&) noexcept -> Chunk& = default;

    static auto calculate_chunk_key(const ChunkCoord& position) noexcept -> std::uint64_t;

    [[nodiscard]] auto get_block_at(LocalCoord position) const noexcept -> Block;
    void set_block_at(LocalCoord position, Block block);

    [[nodiscard]] auto get_position() const noexcept -> ChunkCoord;
    void set_position(ChunkCoord position) noexcept;

    [[nodiscard]] auto get_chunk_key() const noexcept -> std::uint64_t;
    [[nodiscard]] auto is_empty() const noexcept -> bool;

    void generate_mesh(const World& world);
    void render() const noexcept;
};

#endif // VOXIDE_CHUNK_HEADER
