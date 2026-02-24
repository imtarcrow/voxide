#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_video.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <utility>
#include <window.hpp>

#include "glad/glad.h"

Window::Window(const std::string& title, int width, int height, SDL_WindowFlags flags)
{

    spdlog::trace("Creating Window ('{}') w={}, h={}, flags={}", title, width, height, flags);

    this->window_handle = SDL_CreateWindow(title.c_str(), width, height, flags | SDL_WINDOW_OPENGL);
    if (this->window_handle == nullptr) {
        spdlog::error("Failed to create SDL3 Window!");
        throw std::runtime_error("Failed to create SDL3 Window!");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    this->glcontext = SDL_GL_CreateContext(this->window_handle);
    if (this->glcontext == nullptr) {
        spdlog::error("Failed to create SDL3 OpenGL context!");
        throw std::runtime_error("Failed to create SDL3 OpenGL context!");
    }

    SDL_GL_MakeCurrent(this->window_handle, this->glcontext);
    SDL_GL_SetSwapInterval(1);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0) {
        spdlog::error("Failed to initialize GLAD!");
        throw std::runtime_error("Failed to initialize GLAD!");
    }
}

Window::~Window()
{
    spdlog::trace("Destroying Window {}", fmt::ptr(this->window_handle));

    assert(this->glcontext != nullptr);
    SDL_GL_DestroyContext(this->glcontext);

    assert(this->window_handle != nullptr);
    SDL_DestroyWindow(this->window_handle);
}

[[nodiscard]] auto Window::get_handle() const noexcept -> SDL_Window*
{
    return this->window_handle;
}

[[nodiscard]] auto Window::get_context() const noexcept -> SDL_GLContext
{
    return this->glcontext;
}

void Window::handle_event(SDL_WindowEvent event) noexcept
{
    switch (event.type) {
    case SDL_EVENT_WINDOW_SHOWN:
        this->visible = true;
        spdlog::trace("Setting window visibility to {}", this->visible);
        break;
    case SDL_EVENT_WINDOW_HIDDEN:
        this->visible = false;
        spdlog::trace("Setting window visibility to {}", this->visible);
        break;
    case SDL_EVENT_WINDOW_MOVED:
        this->position = { event.data1, event.data2 };
        spdlog::trace("Window moved to x={}, y={}", event.data1, event.data2);
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        this->size = { event.data1, event.data2 };
        glViewport(0, 0, event.data1, event.data2);
        spdlog::trace("Window resized to w={}, h={}", event.data1, event.data2);
        break;
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        this->fullscreen = true;
        spdlog::trace("Setting window fullscreen to {}", this->fullscreen);
        break;
    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        this->fullscreen = false;
        spdlog::trace("Setting window fullscreen to {}", this->fullscreen);
        break;
    default:
        break;
    }
}

void Window::set_visible(bool visible) noexcept
{
    if (visible) {
        SDL_ShowWindow(this->window_handle);
    }
    else {
        SDL_HideWindow(this->window_handle);
    }
}

void Window::set_title(const std::string& title) noexcept
{
    SDL_SetWindowTitle(this->window_handle, title.c_str());
    this->title = title;
}

void Window::set_size(int width, int height) noexcept
{
    SDL_SetWindowSize(this->window_handle, width, height);
}

void Window::set_position(int xpos, int ypos) noexcept
{
    SDL_SetWindowPosition(this->window_handle, xpos, ypos);
}

void Window::set_resizable(bool resizable) noexcept
{
    SDL_SetWindowResizable(this->window_handle, resizable);
    this->resizable = resizable;
}

void Window::set_fullscreen(bool fullscreen) noexcept
{
    SDL_SetWindowFullscreen(this->window_handle, fullscreen);
}

void Window::set_always_on_top(bool always_on_top) noexcept
{
    SDL_SetWindowAlwaysOnTop(this->window_handle, always_on_top);
    this->always_on_top = always_on_top;
}

auto Window::get_size() const noexcept -> std::pair<int, int>
{
    return this->size;
}

auto Window::get_width() const noexcept -> int
{
    return this->size.first;
}

auto Window::get_height() const noexcept -> int
{
    return this->size.second;
}

auto Window::get_position() const noexcept -> std::pair<int, int>
{
    return this->position;
}

auto Window::get_xpos() const noexcept -> int
{
    return this->position.first;
}

auto Window::get_ypos() const noexcept -> int
{
    return this->position.second;
}

auto Window::is_visible() const noexcept -> bool
{
    return this->visible;
}

auto Window::is_resizable() const noexcept -> bool
{
    return this->resizable;
}

auto Window::is_fullscreen() const noexcept -> bool
{
    return this->fullscreen;
}

auto Window::is_always_on_top() const noexcept -> bool
{
    return this->always_on_top;
}
