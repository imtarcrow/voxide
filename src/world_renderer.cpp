#include "world_renderer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "world.hpp"

WorldRenderer::WorldRenderer(World* world, Camera* camera)
    : world(world)
    , camera(camera)
{
    this->program = std::make_unique<ShaderProgram>("./assets/shader/vertex.glsl", "./assets/shader/fragment.glsl");
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

    this->program->set_uniform("view", this->camera->get_view_matrix());
    this->program->set_uniform("projection", this->camera->get_projection_matrix());

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
