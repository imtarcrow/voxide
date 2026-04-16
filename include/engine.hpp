#pragma once

#include <memory>
#include <stb_image.h>
#include <vector>

#include "window.hpp"
#include "world.hpp"
#include "world_renderer.hpp"

constexpr int DEFAULT_WINDOW_WIDTH = 1024;
constexpr int DEFAULT_WINDOW_HEIGHT = 768;

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
    std::unique_ptr<World> world;

    std::unique_ptr<WorldRenderer> world_renderer;

    FrameData frame_data;
    float total_time_passed = 0.0F;
    float time_since_last_log = 0.0F;

    bool checkbox_is_ticked = false;
    bool checkbox_was_ticked = false;

    bool checkbox2_is_ticked = false;
    bool checkbox2_was_ticked = false;

    bool should_quit = false;

    void initialize_imgui() noexcept;

    void prepare_frame() noexcept;
    void end_frame() noexcept;

    void update_frametimes() noexcept;

    void process_events() noexcept;

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
