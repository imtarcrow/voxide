#pragma once
#include <unordered_map>
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

class Engine
{

private:
    std::unique_ptr<Window> window;
    std::unique_ptr<ShaderProgram> program;
    std::unique_ptr<Camera> camera;

    std::unordered_map<std::uint64_t, Chunk> chunks;

    std::unique_ptr<Chunk> debug_chunk;

    FrameData frame_data;
    float total_time_passed = 0.0F;
    float time_since_last_log = 0.0F;

    bool should_quit = false;

    void initialize_imgui() noexcept;

    void prepare_frame() noexcept;
    void end_frame() noexcept;

    void update_frametimes() noexcept;

    void process_events() noexcept;

    void handle_movement(float delta_time) noexcept;

public:
    Engine();
    ~Engine();

    // disable copying
    Engine(const Engine&) = delete;
    auto operator=(const Engine&) -> Engine& = delete;

    // disable moving
    Engine(const Engine&&) = delete;
    auto operator=(const Engine&&) -> Engine& = delete;

    void run();
    void init();
};

#endif // !VOXIDE_GAME_HEADER
