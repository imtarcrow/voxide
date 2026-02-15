#include "game.hpp"

#include "glad/glad.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <stdexcept>


Game::Game() {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Failed to initialize SDL3");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetSwapInterval(1);

    this->window = std::make_unique<Window>(DEFAULT_WIDTH, DEFAULT_HEIGHT,
                                            "test window", SDL_WINDOW_OPENGL);

    if (this->window == nullptr) {
        throw std::runtime_error("Failed to create SDL3 window!");
    }

    this->context = SDL_GL_CreateContext(this->window->handle());
    if (this->context == nullptr) {
        throw std::runtime_error("Failed to create OpenGL context!");
    }

    SDL_GL_MakeCurrent(window->handle(), context);

    if (gladLoadGLLoader(
            reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0) {
        throw std::runtime_error("Failed to initialize GLAD!");
    }

    auto [width, height] = this->window->get_size();
    glViewport(0, 0, width, height);

    this->debug_renderer = SDL_CreateRenderer(this->window->handle(), nullptr);
}

Game::~Game() {

    SDL_GL_DestroyContext(this->context);
    if (this->debug_renderer != nullptr) {
        SDL_DestroyRenderer(this->debug_renderer);
        this->debug_renderer = nullptr;
    }

    SDL_Quit();
}

void Game::run() {

    bool should_quit = false;
    SDL_Event event;
    while (!should_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                should_quit = true;
            }
        }

        auto [width, height] = this->window->get_size();

        SDL_SetRenderDrawColor(debug_renderer, 0, 0, 0, 255);
        SDL_RenderClear(debug_renderer);
        SDL_SetRenderDrawColor(debug_renderer, 255, 0, 0, 255);
        SDL_RenderLine(debug_renderer, 0, 0, static_cast<float>(width),
                       static_cast<float>(height));
        SDL_RenderPresent(debug_renderer);
    }
}
