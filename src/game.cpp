#include "game.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
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

    auto [width, height] = this->window->get_size();
    glViewport(WINDOW_PADDING, WINDOW_PADDING, width - (WINDOW_PADDING * 2), height - (WINDOW_PADDING * 2));

    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(GLfloat), verticies.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    this->program = std::make_unique<ShaderProgram>("./assets/shader/vertex.glsl", "./assets/shader/fragment.glsl");

    glEnable(GL_CULL_FACE);
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
            if (event.type == SDL_EVENT_QUIT) {
                should_quit = true;
            }
        }

        ticks++;

        glEnable(GL_DEPTH_TEST);

        float red = std::sin(static_cast<float>(ticks) / 100.0F);
        float green = std::cos(static_cast<float>(ticks) / 100.0F);
        float blue = std::sin(static_cast<float>(ticks) / 200.0F);

        glClearColor((red + 1) / 2, (green + 1) / 2, (blue + 1) / 2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!this->program->use()) {
            spdlog::error("Failed to use Shader Program");
        }

        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        SDL_GL_SwapWindow(this->window->get_handle());
        ::SDL_Delay(1);
    }
}
