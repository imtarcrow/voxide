#pragma once
#ifndef VOXIDE_COORDINATES_HEADER
#define VOXIDE_COORDINATES_HEADER

#include <glm/glm.hpp>

template <typename Tag> struct Coord
{
    int x;
    int y;
    int z;

    explicit Coord(glm::ivec3 position)
        : x(position.x)
        , y(position.y)
        , z(position.z)
    {
    }
    explicit Coord(int xpos, int ypos, int zpos)
        : x(xpos)
        , y(ypos)
        , z(zpos)
    {
    }

    auto operator+(const Coord& other) const -> Coord
    {
        return Coord(x + other.x, y + other.y, z + other.z);
    }
    auto operator-(const Coord& other) const -> Coord
    {
        return Coord(x - other.x, y - other.y, z - other.z);
    }
    auto operator==(const Coord& other) const -> bool
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }

    [[nodiscard]] auto as_vec() const noexcept -> glm::ivec3
    {
        return {x, y, z};
    }
};

struct WorldTag
{
};
struct ChunkTag
{
};
struct LocalTag
{
};

using WorldCoord = Coord<WorldTag>;
using ChunkCoord = Coord<ChunkTag>;
using LocalCoord = Coord<LocalTag>;

namespace CoordConvert {
    auto world_to_chunk(WorldCoord world_position) -> ChunkCoord;
    auto world_to_local(WorldCoord world_position) -> LocalCoord;
    auto local_to_world(LocalCoord local_position, ChunkCoord chunk_position) -> WorldCoord;
}

#endif
