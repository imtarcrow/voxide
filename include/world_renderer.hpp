#pragma once
#include "camera.hpp"
#include "shader_program.hpp"
#include "world.hpp"

class WorldRenderer
{

private:
    World* world;
    Camera* camera;

    std::unique_ptr<ShaderProgram> program;
    std::unordered_map<std::uint64_t, std::unique_ptr<ChunkMesh>> meshes;

    void process_events();

public:
    WorldRenderer(World* world, Camera* camera);

    void render();
};
