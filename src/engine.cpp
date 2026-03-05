#include "engine.hpp"

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

Engine::Engine()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Failed to initialize SDL3");
    }
}

Engine::~Engine()
{
    SDL_Quit();
}

void Engine::initialize_imgui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& imgui_io = ImGui::GetIO();
    imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL3_InitForOpenGL(this->window->get_window_handle(), this->window->get_context());
    ImGui_ImplOpenGL3_Init();
}

void Engine::init()
{
    this->window = std::make_unique<Window>("test window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    this->program = std::make_unique<ShaderProgram>("./assets/shader/vertex.glsl", "./assets/shader/fragment.glsl");
    this->camera = std::make_unique<Camera>(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F), 90.0F,
                                            static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT));
    this->chunk = std::make_unique<Chunk>(glm::ivec3(0, 0, 0));
    this->chunk1 = std::make_unique<Chunk>(glm::ivec3(1, 1, 1));

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    this->window->set_capturing_mouse(true);
    this->initialize_imgui();
}

void Engine::handle_movement(float delta_time) noexcept
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

void Engine::update_frametimes() noexcept
{
    this->frame_data.delta_time = static_cast<float>((SDL_GetTicksNS() - this->frame_data.last_time_ns)) / 1000000000.0F;
    this->frame_data.last_time_ns = SDL_GetTicksNS();
    this->frame_data.frame_times.push_back(this->frame_data.delta_time);

    this->total_time_passed += this->frame_data.delta_time;
    this->time_since_last_log += this->frame_data.delta_time;

    this->window->set_title(std::format("{:.2f}s passed | test window", this->total_time_passed));

    if (this->time_since_last_log > 5.0F) {
        float average_frame_time = 0.0F;
        for (auto it = this->frame_data.frame_times.begin(); it <= this->frame_data.frame_times.end(); it++) {
            average_frame_time += *it;
        }

        average_frame_time /= static_cast<float>(this->frame_data.frame_times.size());

        spdlog::trace("{} frames rendered in 5.0 seconds. average frametime: {:.4f}, FPS: {}", this->frame_data.frame_times.size(),
                      average_frame_time, 1 / average_frame_time);
        this->time_since_last_log -= 5.0F;
        this->frame_data.frame_times.clear();
    }
}

void Engine::process_events() noexcept
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        // Send all events to imgui, if the camera is not being controlled
        if (!this->window->is_capturing_mouse()) {
            ImGui_ImplSDL3_ProcessEvent(&event);
        }

        // Send all window related events to the window
        if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
            this->window->handle_event(event.window);
        }

        if (event.type == SDL_EVENT_MOUSE_MOTION && this->window->is_capturing_mouse()) {
            this->camera->process_mouse_movement(event.motion.xrel, -event.motion.yrel, true);
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
            this->window->set_capturing_mouse(!this->window->is_capturing_mouse());
        }
        if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            this->camera->set_aspect_ratio(static_cast<float>(event.window.data1) / static_cast<float>(event.window.data2));
        }

        if (event.type == SDL_EVENT_QUIT) {
            this->should_quit = true;
        }
    }
}

void Engine::prepare_frame() noexcept
{
    this->update_frametimes();

    // prepare imgui for the framestart
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::end_frame() noexcept
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(this->window->get_window_handle());
}

void Engine::run()
{

    this->frame_data.last_time_ns = SDL_GetTicksNS();

    while (!this->should_quit) {

        this->prepare_frame();

        this->process_events();

        if (this->window->is_capturing_mouse()) {
            this->handle_movement(this->frame_data.delta_time);
        }

        ImGui::Begin("Info", nullptr, 0);
        ImGui::Text("Position: X: %.2f Y: %.2f Z: %.2f", this->camera->get_x(), this->camera->get_y(), this->camera->get_z());
        ImGui::Text("Orientation: Yaw: %.2f Pitch: %.2f", this->camera->get_yaw(), this->camera->get_pitch());
        ImGui::End();

        if (!this->window->is_capturing_mouse()) {
            ImGui::Begin("Settings", nullptr, 0);
            ImGui::End();
        }

        if (!this->program->use()) {
            spdlog::error("Failed to use Shader Program");
        }

        this->program->set_uniform("view", this->camera->get_view_matrix());
        this->program->set_uniform("projection", this->camera->get_projection_matrix());

        glm::ivec3 position = this->chunk->get_position();
        auto model = glm::mat4(1.0F);
        model = glm::translate(model, { position.x * CHUNK_SIZE_X, position.y * CHUNK_SIZE_Y, position.z * CHUNK_SIZE_Z });

        program->set_uniform("model", model);

        this->chunk->render();

        position = this->chunk1->get_position();
        model = glm::mat4(1.0F);
        model = glm::translate(model, { position.x * CHUNK_SIZE_X, position.y * CHUNK_SIZE_Y, position.z * CHUNK_SIZE_Z });

        program->set_uniform("model", model);
        this->chunk1->render();

        this->end_frame();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // needed to avoid segmentation fault when force quitting
    SDL_SetWindowRelativeMouseMode(this->window->get_window_handle(), false);
}
