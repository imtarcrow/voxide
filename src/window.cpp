#include <SDL3/SDL_video.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <utility>
#include <window.hpp>

#include "glad/glad.h"

Window::Window(const std::string& title, int width, int height, SDL_WindowFlags flags)
{

    spdlog::info("Creating window '{}' with dimensions {}x{}", title, width, height);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    this->window = SDL_CreateWindow(title.c_str(), width, height, flags | SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        spdlog::error("Failed to create SDL3 Window!");
        throw std::runtime_error("Failed to create SDL3 Window!");
    }

    this->context = SDL_GL_CreateContext(this->window);
    if (context == nullptr) {
        spdlog::error("Failed to create SDL3 OpenGL context!");
        throw std::runtime_error("Failed to create SDL3 OpenGL context!");
    }

    SDL_GL_MakeCurrent(this->window, this->context);
    SDL_GL_SetSwapInterval(1);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0) {
        spdlog::error("Failed to initialize GLAD!");
        throw std::runtime_error("Failed to initialize GLAD!");
    }
}

Window::~Window()
{
    if (this->context != nullptr) {
        SDL_GL_DestroyContext(this->context);
    }

    if (this->window != nullptr) {
        SDL_DestroyWindow(this->window);
    }
}

auto Window::get_size() const noexcept -> std::pair<int, int>
{
    int width = 0;
    int height = 0;

    SDL_GetWindowSize(this->window, &width, &height);

    return { width, height };
}

[[nodiscard]] auto Window::get_handle() const noexcept -> SDL_Window*
{
    return this->window;
}

[[nodiscard]] auto Window::get_context() const noexcept -> SDL_GLContext
{
    return this->context;
}
