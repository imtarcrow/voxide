#include <memory>
#include <print>
#include <SDL3/SDL.h>
#include "window.hpp"

auto main() -> int {

    std::println("Starting...");

    if(!SDL_Init(SDL_INIT_VIDEO)) {return 1;}

    std::unique_ptr<Window> window = std::make_unique<Window>(800, 800, "test window");

    SDL_Renderer* renderer = SDL_CreateRenderer(window->handle(), nullptr);

    bool should_quit = false;
    SDL_Event event;
    while (!should_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                should_quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderLine(renderer, 0, 0, 800, 800);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    return 0;
}
