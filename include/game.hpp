#pragma once
#ifndef VOXIDE_GAME_HEADER
#define VOXIDE_GAME_HEADER

#include <memory>
#include <vector>

#include "camera.hpp"
#include "chunk.hpp"
#include "shader_program.hpp"
#include "window.hpp"

constexpr int DEFAULT_WIDTH = 1024;
constexpr int DEFAULT_HEIGHT = 768;

using FrameData = struct FrameData
{
    float time_passed;
    std::vector<float> frame_times;
};

class Game
{

private:
    std::unique_ptr<Window> window;
    std::unique_ptr<ShaderProgram> program;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Chunk> chunk;

    FrameData frame_data;

    void handle_movement(float delta_time) noexcept;
    void init_dear_imgui() noexcept;

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
