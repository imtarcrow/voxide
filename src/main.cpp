#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <memory>

#include "game.hpp"

auto main() -> int
{
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->run();

    return 0;
}
