#include "game.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
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
    this->camera = std::make_unique<Camera>(glm::vec3(0.0F, 0.0F, 2.0F), glm::vec2(-90.0F, 0.0F), 90.0F,
                                            static_cast<float>(DEFAULT_WIDTH) / static_cast<float>(DEFAULT_HEIGHT));

    SDL_SetWindowRelativeMouseMode(this->window->get_handle(), true);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void Game::run()
{
    unsigned long long ticks = 0;

    bool should_quit = false;
    SDL_Event event;
    while (!should_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                this->camera->process_mouse_movement(event.motion.xrel, -event.motion.yrel, true);
            }
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
        const bool* keys = SDL_GetKeyboardState(nullptr);

        if (keys[SDL_SCANCODE_W]) {
            this->camera->set_position(this->camera->get_position() + this->camera->get_front_vector() * 0.02F);
        }

        if (keys[SDL_SCANCODE_S]) {
            this->camera->set_position(this->camera->get_position() - this->camera->get_front_vector() * 0.02F);
        }

        if (keys[SDL_SCANCODE_D]) {
            this->camera->set_position(this->camera->get_position() + this->camera->get_right_vector() * 0.02F);
        }

        if (keys[SDL_SCANCODE_A]) {
            this->camera->set_position(this->camera->get_position() - this->camera->get_right_vector() * 0.02F);
        }

        if (keys[SDL_SCANCODE_SPACE]) {
            this->camera->set_position(this->camera->get_position() + glm::vec3(0.0, 1.0, 0.0) * 0.02F);
        }

        if (keys[SDL_SCANCODE_LSHIFT]) {
            this->camera->set_position(this->camera->get_position() - glm::vec3(0.0, 1.0, 0.0) * 0.02F);
        }

        glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(this->VAO);

        if (!this->program->use()) {
            spdlog::error("Failed to use Shader Program");
        }

        this->program->set_uniform("view", this->camera->get_view_matrix());
        this->program->set_uniform("projection", this->camera->get_projection_matrix());

        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                for (int z = 0; z < 10; z++) {
                    auto model = glm::mat4(1.0F);
                    model = glm::translate(model, glm::vec3(x, y, z));

                    this->program->set_uniform("model", model);
                    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
                }
            }
        }

        SDL_GL_SwapWindow(this->window->get_handle());
        ::SDL_Delay(1);
    }

    SDL_SetWindowRelativeMouseMode(this->window->get_handle(), false);

    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
    glDeleteVertexArrays(1, &this->VAO);

    this->VAO = this->VBO = this->EBO = 0;
}
