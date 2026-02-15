#include "window.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <print>

constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 800;

auto main() -> int {

    std::println("Starting...");

    if(!SDL_Init(SDL_INIT_VIDEO)) {return 1;}

    std::unique_ptr<Window> window = std::make_unique<Window>(DEFAULT_WIDTH, DEFAULT_HEIGHT, "test window", SDL_WINDOW_RESIZABLE);

    SDL_Renderer* renderer = SDL_CreateRenderer(window->handle(), nullptr);

    bool should_quit = false;
    SDL_Event event;
    while (!should_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                should_quit = true;
            }
        }

        auto [width, height] = window->get_size();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderLine(renderer, 0, 0, static_cast<float>(width), static_cast<float>(height));
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    return 0;
}
