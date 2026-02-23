#pragma once
#ifndef VOXIDE_GAME_HEADER
#define VOXIDE_GAME_HEADER

#include <array>
#include <memory>

#include "glad/glad.h"
#include "shader_program.hpp"
#include "window.hpp"

constexpr int DEFAULT_WIDTH = 1024;
constexpr int DEFAULT_HEIGHT = 768;

constexpr std::array<GLfloat, 9> verticies = { 0, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0 };

constexpr std::array<GLuint, 3> indices = { 0, 1, 2 };

class Game
{

private:
    std::unique_ptr<Window> window;

    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint VAO = 0;

    std::unique_ptr<ShaderProgram> program;

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
