#include "chunk.hpp"
#include "coordinates.hpp"

namespace CoordConvert {
    auto world_to_chunk(WorldCoord world_position) -> ChunkCoord
    {
        auto floordiv = [](int a, int b) -> int
        {
            return a / b - (a % b != 0 && (a ^ b) < 0);
        };
        return ChunkCoord(floordiv(world_position.x, CHUNK_SIZE_X),
                          floordiv(world_position.y, CHUNK_SIZE_Y),
                          floordiv(world_position.z, CHUNK_SIZE_Z));
    }

    auto world_to_local(WorldCoord world_position) -> LocalCoord
    {
        auto floormod = [](int a, int b) -> int
        {
            return ((a % b) + b) % b;
        };
        return LocalCoord(floormod(world_position.x, CHUNK_SIZE_X),
                          floormod(world_position.y, CHUNK_SIZE_Y),
                          floormod(world_position.z, CHUNK_SIZE_Z));
    }

    auto local_to_world(LocalCoord local_position, ChunkCoord chunk_position) -> WorldCoord
    {
        return WorldCoord((chunk_position.x * CHUNK_SIZE_X) + local_position.x,
                          (chunk_position.y * CHUNK_SIZE_Y) + local_position.y,
                          (chunk_position.z * CHUNK_SIZE_Z) + local_position.z);
    }
}
