#include "game.hpp"
#include "window.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <print>


auto main() -> int {

    std::println("Starting...");

    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->run();

    return 0;
}
