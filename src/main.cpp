#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "engine.hpp"

auto main() -> int
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Starting...");
    std::unique_ptr<Engine> game = std::make_unique<Engine>();

    game->init();
    game->run();

    return 0;
}
