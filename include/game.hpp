#pragma once
#ifndef VOXIDE_GAME_HEADER
#define VOXIDE_GAME_HEADER

#include <memory>
#include <vector>

#include "camera.hpp"
#include "chunk.hpp"
#include "shader_program.hpp"
#include "window.hpp"

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

using FrameData = struct FrameData
{
    float delta_time;
    unsigned long last_time_ns;

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
    float total_time_passed;
    float time_since_last_log;

    bool should_quit = false;

    void initialize_imgui() noexcept;
    void prepare_frame() noexcept;
    void end_frame() noexcept;

    void update_frametimes() noexcept;

    void process_events() noexcept;

    void handle_movement(float delta_time) noexcept;

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
