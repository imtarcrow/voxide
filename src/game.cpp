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
#include <format>
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

void Game::init_dear_imgui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& imgui_io = ImGui::GetIO();
    imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL3_InitForOpenGL(this->window->get_handle(), this->window->get_context());
    ImGui_ImplOpenGL3_Init();
}

void Game::init()
{
    this->window = std::make_unique<Window>("test window", DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE);

    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, verts2.size() * sizeof(GLfloat), verts2.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds2.size() * sizeof(GLuint), inds2.data(), GL_STATIC_DRAW);

    this->program = std::make_unique<ShaderProgram>("./assets/shader/vertex.glsl", "./assets/shader/fragment.glsl");
    this->camera = std::make_unique<Camera>(glm::vec3(2.0F, 0.0F, 0.0F), glm::vec2(180.0F, 0.0F), 90.0F,
                                            static_cast<float>(DEFAULT_WIDTH) / static_cast<float>(DEFAULT_HEIGHT));

    this->chunk = std::make_unique<Chunk>(glm::ivec3(0, 0, 0));

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    this->window->set_capturing_mouse(true);
    this->init_dear_imgui();
}

void Game::handle_movement(float delta_time) noexcept
{
    const bool* keys = SDL_GetKeyboardState(nullptr);

    const float sensitivity = 20.0F;

    if (keys[SDL_SCANCODE_W]) {
        this->camera->set_position(this->camera->get_position() + this->camera->get_front_vector() * sensitivity * delta_time);
    }

    if (keys[SDL_SCANCODE_S]) {
        this->camera->set_position(this->camera->get_position() - this->camera->get_front_vector() * sensitivity * delta_time);
    }

    if (keys[SDL_SCANCODE_D]) {
        this->camera->set_position(this->camera->get_position() + this->camera->get_right_vector() * sensitivity * delta_time);
    }

    if (keys[SDL_SCANCODE_A]) {
        this->camera->set_position(this->camera->get_position() - this->camera->get_right_vector() * sensitivity * delta_time);
    }

    if (keys[SDL_SCANCODE_SPACE]) {
        this->camera->set_position(this->camera->get_position() + glm::vec3(0.0, 1.0, 0.0) * sensitivity * delta_time);
    }

    if (keys[SDL_SCANCODE_LSHIFT]) {
        this->camera->set_position(this->camera->get_position() - glm::vec3(0.0, 1.0, 0.0) * sensitivity * delta_time);
    }
}

void Game::run()
{
    Uint64 last_time = SDL_GetTicks();

    int xcount = 1;
    int ycount = 1;
    int zcount = 1;

    bool should_quit = false;
    SDL_Event event;
    while (!should_quit) {
        while (SDL_PollEvent(&event)) {
            if (!this->window->is_capturing_mouse()) {
                ImGui_ImplSDL3_ProcessEvent(&event);
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION && this->window->is_capturing_mouse()) {
                this->camera->process_mouse_movement(event.motion.xrel, -event.motion.yrel, true);
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                this->window->set_capturing_mouse(!this->window->is_capturing_mouse());
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

        if (!this->window->is_capturing_mouse()) {
            ImGui::Begin("Settings", nullptr, 0);

            ImGui::DragInt("X", &xcount, 1, 0, 100);
            ImGui::DragInt("Y", &ycount, 1, 0, 100);
            ImGui::DragInt("Z", &zcount, 1, 0, 100);

            ImGui::End();
        }

        ImGui::Begin("Info", nullptr, 0);
        ImGui::Text("Position: X: %.2f Y: %.2f Z: %.2f", this->camera->get_x(), this->camera->get_y(), this->camera->get_z());
        ImGui::Text("Orientation: Yaw: %.2f Pitch: %.2f", this->camera->get_yaw(), this->camera->get_pitch());
        ImGui::End();

        Uint64 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;

        this->frame_data.frame_times.push_back(delta_time);
        this->frame_data.time_passed += delta_time;

        if (this->frame_data.time_passed > 5.0F) {
            float average_time = 0;

            for (auto it = this->frame_data.frame_times.begin(); it <= this->frame_data.frame_times.end(); it++) {
                average_time += *it;
            }

            average_time /= static_cast<float>(this->frame_data.frame_times.size());

            spdlog::trace("{} frames rendered in 5.0 seconds. average frametime: {:.4f}, FPS: {}", this->frame_data.frame_times.size(),
                          average_time, 1 / average_time);
            this->frame_data.time_passed -= 5.0F;
            this->frame_data.frame_times.clear();
        }

        last_time = current_time;

        if (this->window->is_capturing_mouse()) {
            this->handle_movement(delta_time);
        }

        glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(this->VAO);

        if (!this->program->use()) {
            spdlog::error("Failed to use Shader Program");
        }

        this->program->set_uniform("view", this->camera->get_view_matrix());
        this->program->set_uniform("projection", this->camera->get_projection_matrix());

        for (int xpos = 0; xpos < xcount; xpos++) {
            for (int ypos = 0; ypos < ycount; ypos++) {
                for (int zpos = 0; zpos < zcount; zpos++) {
                    auto model = glm::mat4(1.0F);
                    model = glm::translate(model, glm::vec3(xpos, ypos, zpos));

                    this->program->set_uniform("model", model);
                    // glDrawElements(GL_TRIANGLES, inds2.size(), GL_UNSIGNED_INT, nullptr);
                }
            }
        }

        glBindVertexArray(this->chunk->VAO);
        glDrawElements(GL_TRIANGLES, this->chunk->indicies_size, GL_UNSIGNED_INT, nullptr);

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
