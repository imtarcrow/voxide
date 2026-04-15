#pragma once
#include "shader_program.hpp"
#include "world.hpp"

class WorldRenderer
{

private:
    World* world;

    GLuint texture_atlas_handle = 0;  

    std::unique_ptr<ShaderProgram> program;
    std::unordered_map<std::uint64_t, std::unique_ptr<ChunkMesh>> meshes;

    auto load_texture_atlas() -> bool;
    void process_events();

public:
    WorldRenderer(World* world);
    ~WorldRenderer();
    
    // disable copying
    WorldRenderer(const WorldRenderer&) = delete;
    auto operator=(const WorldRenderer&) -> WorldRenderer& = delete;

    // disable moving
    WorldRenderer(const WorldRenderer&&) = delete;
    auto operator=(const WorldRenderer&&) -> WorldRenderer& = delete;

    void render();
    auto reload_texture_atlas() -> bool;
};
