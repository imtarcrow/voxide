#include "glad/glad.h"
#include <SDL3/SDL_video.h>
#include <stdexcept>
#include <utility>
#include <window.hpp>

Window::Window(const std::string &title, int width, int height,
               SDL_WindowFlags flags) {

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    this->window = SDL_CreateWindow(title.c_str(), width, height, flags | SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        throw std::runtime_error("Failed to create SDL3 window!");
    }

    this->context = SDL_GL_CreateContext(this->window);
    if (context == nullptr) {
        throw std::runtime_error("Failed to create SDL3 OpenGL context!");
    }

    SDL_GL_MakeCurrent(this->window, this->context);
    SDL_GL_SetSwapInterval(1);

    if (gladLoadGLLoader(
            reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0) {
        throw std::runtime_error("Failed to initialize GLAD!");
    }
}

Window::~Window() {
    if(this->context != nullptr) {
        SDL_GL_DestroyContext(this->context);
    }

    if (this->window != nullptr) {
        SDL_DestroyWindow(this->window);
    }
}

auto Window::get_size() const noexcept -> std::pair<int, int> {

    int width = 0;
    int height = 0;

    SDL_GetWindowSize(this->window, &width, &height);

    return {width, height};
}

[[nodiscard]] auto Window::get_handle() const noexcept -> SDL_Window* {
    return this->window;
}

[[nodiscard]] auto Window::get_context() const noexcept -> SDL_GLContext {
    return this->context;
}
