#pragma once
#ifndef VOXIDE_CHUNK_HEADER
#define VOXIDE_CHUNK_HEADER

#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <optional>

#include "block.hpp"
#include "chunk_mesh.hpp"
#include "coordinates.hpp"

constexpr std::uint8_t CHUNK_SIZE_X = 16;
constexpr std::uint8_t CHUNK_SIZE_Y = 16;
constexpr std::uint8_t CHUNK_SIZE_Z = 16;
constexpr std::uint16_t CHUNK_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

class Chunk
{

private:
    std::array<Block, CHUNK_SIZE> blocks = { Block::Air };

    ChunkCoord position;
    std::unique_ptr<ChunkMesh> mesh;

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

    [[nodiscard]] auto get_block_at(LocalCoord position) const noexcept -> std::optional<Block>;
    void set_block_at(LocalCoord position, Block block) noexcept;

    [[nodiscard]] auto get_position() const noexcept -> ChunkCoord;
    void set_position(ChunkCoord position) noexcept;

    [[nodiscard]] auto get_chunk_key() const noexcept -> std::uint64_t;
    [[nodiscard]] static auto is_inside_chunk(LocalCoord position) noexcept -> bool;

    void generate_mesh(const World& world);
    void render() const noexcept;
};

#endif // VOXIDE_CHUNK_HEADER
