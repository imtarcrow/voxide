#include <SDL3/SDL_video.h>
#include <stdexcept>
#include <utility>
#include <window.hpp>

Window::Window(int width, int height, const std::string& title, SDL_WindowFlags flags) {
    SDL_Window* window = SDL_CreateWindow(title.c_str(), width, height, flags);

    if(window == nullptr) {
        throw std::runtime_error("Failed to create a window!");
    }

    this->window = window;
}

Window::~Window() {
    if(this->window != nullptr) {
        SDL_DestroyWindow(this->window);
    }
}

auto Window::get_size() const noexcept -> std::pair<int, int> {

    int width = 0; int height = 0;
    
    SDL_GetWindowSize(this->window, &width, &height);

    return {width, height};
}

[[nodiscard]] auto Window::handle() const noexcept -> SDL_Window* { return this->window; }
