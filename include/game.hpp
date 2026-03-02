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
    0, 6, 2, 0, 4, 6, 4, 7, 6, 4, 5, 7, 5, 3, 7, 5, 1, 3, 1, 2, 3, 1, 0, 2, 2, 7, 3, 2, 6, 7, 4, 1, 5, 4, 0, 1,
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
