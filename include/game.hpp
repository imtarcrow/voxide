#pragma once
#ifndef VOXIDE_GAME_HEADER
#define VOXIDE_GAME_HEADER

#include <array>
#include <memory>

#include "camera.hpp"
#include "glad/glad.h"
#include "shader_program.hpp"
#include "window.hpp"

constexpr int DEFAULT_WIDTH = 1024;
constexpr int DEFAULT_HEIGHT = 768;

constexpr std::array<GLuint, 36> indices = {
    0, 2, 6, 0, 6, 4, 4, 6, 7, 4, 7, 5, 5, 7, 3, 5, 3, 1, 1, 3, 2, 1, 2, 0, 2, 3, 7, 2, 7, 6, 4, 5, 1, 4, 1, 0,
};


constexpr std::array<GLfloat, 48> verticies = {
    -0.5, -0.5, -0.5, 0.0, 0.0, 0.0, -0.5, -0.5, +0.5, 0.0, 0.0, 1.0, -0.5, +0.5, -0.5, 0.0, 1.0, 0.0, -0.5, +0.5, +0.5, 0.0, 1.0, 1.0,
    +0.5, -0.5, -0.5, 1.0, 0.0, 0.0, +0.5, -0.5, +0.5, 1.0, 0.0, 1.0, +0.5, +0.5, -0.5, 1.0, 1.0, 0.0, +0.5, +0.5, +0.5, 1.0, 1.0, 1.0,
};

class Game
{

private:
    std::unique_ptr<Window> window;
    std::unique_ptr<ShaderProgram> program;
    std::unique_ptr<Camera> camera;

    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint VAO = 0;

public:
    Game();
    ~Game();

    // disable copying
    Game(const Game&) = delete;
    auto operator=(const Game&) -> Game& = delete;

    // disable moving
    Game(const Game&&) = delete;
    auto operator=(const Game&&) -> Game& = delete;

    void run();
    void init();
};

#endif // !VOXIDE_GAME_HEADER
