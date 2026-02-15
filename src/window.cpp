#include <stdexcept>
#include <window.hpp>

Window::Window(int width, int height, const std::string title) {
    SDL_Window* window = SDL_CreateWindow(title.c_str(), width, height, 0);

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

[[nodiscard]] auto Window::handle() const noexcept -> SDL_Window* { return this->window; }
