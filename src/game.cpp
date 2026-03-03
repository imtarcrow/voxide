#include "game.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/imgui.h>
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL3_InitForOpenGL(this->window->get_handle(), this->window->get_context());
    ImGui_ImplOpenGL3_Init();
}

void Game::run()
{

    Uint64 last_time = SDL_GetTicks();
    ImGuiIO& io = ImGui::GetIO();

    float delta_delta_time = 0.0F;

    bool should_quit = false;
    SDL_Event event;
    while (!should_quit) {
        while (SDL_PollEvent(&event)) {

            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_MOUSE_MOTION && !io.WantCaptureMouse) {
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
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        Uint64 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        delta_delta_time += delta_time;

        if (delta_delta_time > 1.0F) {
            spdlog::trace("Delta time: {:.4f} s", delta_time);
            delta_delta_time -= 1.0F;
        }

        const bool* keys = SDL_GetKeyboardState(nullptr);

        const float sensitivity = 20.0F;

        if (keys[SDL_SCANCODE_W] && !io.WantCaptureKeyboard) {
            this->camera->set_position(this->camera->get_position() + this->camera->get_front_vector() * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_S] && !io.WantCaptureKeyboard) {
            this->camera->set_position(this->camera->get_position() - this->camera->get_front_vector() * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_D] && !io.WantCaptureKeyboard) {
            this->camera->set_position(this->camera->get_position() + this->camera->get_right_vector() * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_A] && !io.WantCaptureKeyboard) {
            this->camera->set_position(this->camera->get_position() - this->camera->get_right_vector() * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_SPACE] && !io.WantCaptureKeyboard) {
            this->camera->set_position(this->camera->get_position() + glm::vec3(0.0, 1.0, 0.0) * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_LSHIFT] && !io.WantCaptureKeyboard) {
            this->camera->set_position(this->camera->get_position() - glm::vec3(0.0, 1.0, 0.0) * sensitivity * delta_time);
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
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(this->window->get_handle());

        SDL_Delay(1);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_SetWindowRelativeMouseMode(this->window->get_handle(), false);

    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
    glDeleteVertexArrays(1, &this->VAO);

    this->VAO = this->VBO = this->EBO = 0;
}
