#pragma once
#include <cassert>
#include <cmath>
#include <cstdint>
#include <glm/vec3.hpp>

constexpr uint8_t CHUNK_SIZE_X = 32;
constexpr uint8_t CHUNK_SIZE_Y = 32;
constexpr uint8_t CHUNK_SIZE_Z = 32;
constexpr uint32_t CHUNK_SIZE
    = static_cast<std::uint32_t>(CHUNK_SIZE_X) * static_cast<std::uint32_t>(CHUNK_SIZE_Y) * static_cast<std::uint32_t>(CHUNK_SIZE_Z);

struct LocalCoord
{
    uint8_t x, y, z;

    [[nodiscard]] static auto from(int xpos, int ypos, int zpos) noexcept -> LocalCoord
    {
        assert(xpos >= 0 && xpos < CHUNK_SIZE_X && "LocalCoord x out of range");
        assert(ypos >= 0 && ypos < CHUNK_SIZE_Y && "LocalCoord y out of range");
        assert(zpos >= 0 && zpos < CHUNK_SIZE_Z && "LocalCoord z out of range");
        return LocalCoord { .x = static_cast<uint8_t>(xpos), .y = static_cast<uint8_t>(ypos), .z = static_cast<uint8_t>(zpos) };
    }

    [[nodiscard]] static auto from_vec(glm::vec3 pos) noexcept -> LocalCoord
    {
        assert(pos.x >= 0 && pos.x < CHUNK_SIZE_X && "LocalCoord x out of range");
        assert(pos.y >= 0 && pos.y < CHUNK_SIZE_Y && "LocalCoord y out of range");
        assert(pos.z >= 0 && pos.z < CHUNK_SIZE_Z && "LocalCoord z out of range");
        return LocalCoord { .x = static_cast<uint8_t>(pos.x), .y = static_cast<uint8_t>(pos.y), .z = static_cast<uint8_t>(pos.z) };
    }

    [[nodiscard]] auto operator==(const LocalCoord& other) const noexcept -> bool
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }
};

struct ChunkCoord
{
    int32_t x, y, z;

    [[nodiscard]] auto operator+(const ChunkCoord& other) const noexcept -> ChunkCoord
    {
        return ChunkCoord { .x = this->x + other.x, .y = this->y + other.y, .z = this->z + other.z };
    }

    [[nodiscard]] auto operator-(const ChunkCoord& other) const noexcept -> ChunkCoord
    {
        return ChunkCoord { .x = this->x - other.x, .y = this->y - other.y, .z = this->z - other.z };
    }

    [[nodiscard]] auto operator==(const ChunkCoord& other) const noexcept -> bool
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    [[nodiscard]] auto operator!=(const ChunkCoord& other) const noexcept -> bool
    {
        return !(*this == other);
    }
};

struct ChunkCoordHash
{
    [[nodiscard]] auto operator()(const ChunkCoord& coord) const noexcept -> std::size_t
    {
        std::size_t hash = 0;
        auto mix = [&](int32_t value) { hash ^= std::hash<int32_t> {}(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2); };
        mix(coord.x);
        mix(coord.y);
        mix(coord.z);
        return hash;
    }
};

struct WorldCoord
{
    int32_t x, y, z;

    [[nodiscard]] auto operator==(const WorldCoord& other) const noexcept -> bool
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    [[nodiscard]] auto operator!=(const WorldCoord& other) const noexcept -> bool
    {
        return !(*this == other);
    }

    [[nodiscard]] auto operator+(const WorldCoord& other) const noexcept -> WorldCoord
    {
        return WorldCoord { .x = this->x + other.x, .y = this->y + other.y, .z = this->z + other.z };
    }

    [[nodiscard]] auto operator-(const WorldCoord& other) const noexcept -> WorldCoord
    {
        return WorldCoord { .x = this->x - other.x, .y = this->y - other.y, .z = this->z - other.z };
    }
};

namespace Coords
{

[[nodiscard]] inline auto floor_div(int32_t first, int32_t second) noexcept -> int32_t
{
    return (first / second) - (first % second != 0 && (first ^ second) < 0);
}

[[nodiscard]] inline auto floor_mod(int32_t first, int32_t second) noexcept -> int32_t
{
    return first - (floor_div(first, second) * second);
}

[[nodiscard]] inline auto world_to_chunk(const WorldCoord& coord) noexcept -> ChunkCoord
{
    return ChunkCoord { .x = floor_div(coord.x, CHUNK_SIZE_X),
                        .y = floor_div(coord.y, CHUNK_SIZE_Y),
                        .z = floor_div(coord.z, CHUNK_SIZE_Z) };
}

[[nodiscard]] inline auto world_to_local(const WorldCoord& coord) noexcept -> LocalCoord
{
    return LocalCoord::from(floor_mod(coord.x, CHUNK_SIZE_X), floor_mod(coord.y, CHUNK_SIZE_Y), floor_mod(coord.z, CHUNK_SIZE_Z));
}

[[nodiscard]] inline auto to_world(const ChunkCoord& chunk_coord, const LocalCoord& local_coord) noexcept -> WorldCoord
{
    return WorldCoord { .x = (chunk_coord.x * CHUNK_SIZE_X) + static_cast<int32_t>(local_coord.x),
                        .y = (chunk_coord.y * CHUNK_SIZE_Y) + static_cast<int32_t>(local_coord.y),
                        .z = (chunk_coord.z * CHUNK_SIZE_Z) + static_cast<int32_t>(local_coord.z) };
}

[[nodiscard]] inline auto chunk_origin(const ChunkCoord& coord) noexcept -> WorldCoord
{
    return WorldCoord { .x = coord.x * CHUNK_SIZE_X, .y = coord.y * CHUNK_SIZE_Y, .z = coord.z * CHUNK_SIZE_Z };
}

[[nodiscard]] inline auto from_vec3(const glm::vec3& vec) noexcept -> WorldCoord
{
    return WorldCoord { .x = static_cast<int32_t>(std::floor(vec.x)),
                        .y = static_cast<int32_t>(std::floor(vec.y)),
                        .z = static_cast<int32_t>(std::floor(vec.z)) };
}

} // namespace Coords
