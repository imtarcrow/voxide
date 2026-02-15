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

    this->window = std::make_unique<Window>("test window", DEFAULT_WIDTH, DEFAULT_HEIGHT, 0);

    this->debug_renderer = SDL_CreateRenderer(this->window->get_handle(), nullptr);
}

Game::~Game() {
    if (this->debug_renderer != nullptr) {
        SDL_DestroyRenderer(this->debug_renderer);
        this->debug_renderer = nullptr;
    }

    SDL_Quit();
}

void Game::run() {
    auto [width, height] = this->window->get_size();
    glViewport(0, 0, width, height);
    
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
