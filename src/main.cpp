#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "game.hpp"

auto main() -> int {

    spdlog::info("Starting...");
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->run();

    return 0;
}
