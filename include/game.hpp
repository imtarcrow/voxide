#pragma once
#include "window.hpp"

#include <memory>
#ifndef VOXIDE_GAME_HEADER
#define VOXIDE_GAME_HEADER

constexpr int DEFAULT_WIDTH = 1280;
constexpr int DEFAULT_HEIGHT = 960;

class Game {

  private:
    std::unique_ptr<Window> window;

    SDL_Renderer *debug_renderer = nullptr;

  public:
    Game();
    ~Game();

    // disable copying
    Game(const Game &) = delete;
    auto operator=(const Game &) -> Game & = delete;

    // disable moving
    Game(const Game &&) = delete;
    auto operator=(const Game &&) -> Game & = delete;

    void run();
};


#endif // !VOXIDE_GAME_HEADER
