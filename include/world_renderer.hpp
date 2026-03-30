#pragma once
#include "world.hpp"

class WorldRenderer
{

private:
    World* world;

    std::unordered_map<std::uint64_t, std::unique_ptr<ChunkMesh>> meshes;

public:
    WorldRenderer(World& world);
};

