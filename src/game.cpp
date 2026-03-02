#include "game.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>

#include "glad/glad.h"
#include "shader_program.hpp"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Failed to initialize SDL3");
    }
}

Game::~Game()
{
    SDL_Quit();
}

void Game::init()
{
    this->window = std::make_unique<Window>("test window", DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE);

    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(GLfloat), verticies.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    this->program = std::make_unique<ShaderProgram>("./assets/shader/vertex.glsl", "./assets/shader/fragment.glsl");
    this->camera = std::make_unique<Camera>(90.0F, glm::vec3(0.0F, 0.0F, 2.0F),
                                            static_cast<float>(DEFAULT_WIDTH) / static_cast<float>(DEFAULT_HEIGHT));

    // glEnable(GL_CULL_FACE);
}

void Game::run()
{
    unsigned long long ticks = 0;

    bool should_quit = false;
    SDL_Event event;
    while (!should_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
                this->window->handle_event(event.window);
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                this->camera->set_aspect_ratio(static_cast<float>(event.window.data1) / static_cast<float>(event.window.data2));
            }
            if (event.type == SDL_EVENT_QUIT) {
                should_quit = true;
            }
        }

        ticks++;

        float zpos = std::sin(static_cast<float>(ticks) / 100.0F);
        this->camera->set_z(zpos + 2.0F);

        glEnable(GL_DEPTH_TEST);

        glClearColor((zpos / 2), 1.0F, (zpos / 2) + 1, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!this->program->use()) {
            spdlog::error("Failed to use Shader Program");
        }

        auto model = glm::mat4(1.0F);
        model = glm::rotate(model, glm::radians(static_cast<float>(ticks)), glm::vec3(1.0F, 0.3F, 0.5F)); 

        this->program->set_uniform("model", model); 
        this->program->set_uniform("view", this->camera->get_view_matrix());
        this->program->set_uniform("projection", this->camera->get_projection_matrix());

        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        SDL_GL_SwapWindow(this->window->get_handle());
        ::SDL_Delay(1);
    }
}
