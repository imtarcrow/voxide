#pragma once
#include "glad/glad.h"
#include "window.hpp"

#include <array>
#include <memory>
#ifndef VOXIDE_GAME_HEADER
#define VOXIDE_GAME_HEADER

constexpr int DEFAULT_WIDTH = 1024;
constexpr int DEFAULT_HEIGHT = 768;

constexpr std::array<GLfloat, 48> verticies = {

    // Front face (z = 0.5)
    -0.5F, -0.5F, 0.5F, // Bottom-left
    0.0F, 0.0F, 0.0F,

    0.5F, -0.5F, 0.5F, // Bottom-right
    1.0F, 0.0F, 0.0F,

    0.5F, 0.5F, 0.5F, // Top-right
    0.0F, 1.0F, 0.0F,

    -0.5F, 0.5F, 0.5F, // Top-left
    1.0F, 1.0F, 0.0F,

    // Back face (z = -0.5)

    -0.5F, -0.5F, -0.5F, // Bottom-left
    0.0F, 0.0F, 1.0F,

    0.5F, -0.5F, -0.5F, // Bottom-right
    1.0F, 0.0F, 1.0F,

    0.5F, 0.5F, -0.5F, // Top-right
    0.0F, 1.0F, 1.0F,

    -0.5F, 0.5F, -0.5F, // Top-left
    1.0F, 1.0F, 1.0F};

constexpr std::array<GLuint, 36> indices = {
    // Front face
    0, 1, 2, 0, 2, 3,
    // Back face
    7, 6, 5, 7, 5, 4,
    // Left face
    4, 0, 3, 4, 3, 7,
    // Right face
    1, 5, 6, 1, 6, 2,
    // Top face
    3, 2, 6, 3, 6, 7,
    // Bottom face
    0, 5, 1, 0, 4, 5
};

class Game {

  private:
    std::unique_ptr<Window> window;

    SDL_Renderer *debug_renderer = nullptr;

    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint VAO = 0;

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
    void init();
};


#endif // !VOXIDE_GAME_HEADER
