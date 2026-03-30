#pragma once
#include "world.hpp"
#ifndef VOXIDE_WORLD_RENDERER_HEADER
#define VOXIDE_WORLD_RENDERER_HEADER

class WorldRenderer
{

private:
    World* world;

    std::unordered_map<std::uint64_t, std::unique_ptr<ChunkMesh>> meshes;

public:
    WorldRenderer(World& world);
};

#endif // !VOXIDE_WORLD_RENDERER_HEADER
