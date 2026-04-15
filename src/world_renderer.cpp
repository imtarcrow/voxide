#include "world_renderer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <stb_image.h>

#include "world.hpp"

WorldRenderer::WorldRenderer(World* world)
    : world(world)
{
    this->program = std::make_unique<ShaderProgram>("./assets/shader/vertex.glsl", "./assets/shader/fragment.glsl");

    this->load_texture_atlas();
}

WorldRenderer::~WorldRenderer()
{
    glDeleteTextures(1, &this->texture_atlas_handle);
}

auto WorldRenderer::load_texture_atlas() -> bool
{
    int atlas_width = 0;
    int atlas_height = 0;
    unsigned char* atlas_data = stbi_load("./assets/texture_atlas.png", &atlas_width, &atlas_height, nullptr, 0);

    if (atlas_data == nullptr) {
        spdlog::warn("Failed to load texture atlas file");
        return false;
    }

    // generate texture if it does not exist
    if (this->texture_atlas_handle == 0) {
        glGenTextures(1, &this->texture_atlas_handle);
    }

    glBindTexture(GL_TEXTURE_2D, this->texture_atlas_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlas_width, atlas_height, 0, GL_RGB, GL_UNSIGNED_BYTE, atlas_data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(atlas_data);
    return true;
}

void WorldRenderer::process_events()
{

    auto& events = this->world->get_events();

    while (!events.empty()) {
        auto event = events.front();
        events.pop();

        switch (event.type) {
        case WorldEventType::ChunkLoaded: {
            const auto [iter, inserted] = meshes.emplace(event.hash, std::make_unique<ChunkMesh>(event.position));
            break;
        }
        case WorldEventType::ChunkUnloaded: {
            break;
        }
        case WorldEventType::ChunkDirty: {
            auto& mesh = meshes.at(event.hash);
            mesh->generate(*world->try_get_chunk(event.position), *this->world);
            break;
        }
        default:
            break;
        }
    }
}

void WorldRenderer::render()
{
    this->process_events();

    if (!this->program->use()) {
        spdlog::error("Failed to use Shader Program");
    }

    Camera* camera = this->world->get_local_player()->get_camera();

    this->program->set_uniform("view", camera->get_view_matrix());
    this->program->set_uniform("projection", camera->get_projection_matrix());

    for (auto& entry : this->meshes) {

        ChunkCoord position = entry.second->get_position();
        auto model = glm::mat4(1.0F);

        model = glm::translate(model,
                               { position.x * static_cast<int>(CHUNK_SIZE_X), position.y * static_cast<int>(CHUNK_SIZE_Y),
                                 position.z * static_cast<int>(CHUNK_SIZE_Z) });

        program->set_uniform("model", model);
        entry.second->render();
    }
}

auto WorldRenderer::reload_texture_atlas() -> bool
{
    return this->load_texture_atlas();
}
