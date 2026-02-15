#pragma once
#ifndef VOXIDE_WINDOW_HEADER

#include <SDL3/SDL.h>
#include <string>
#include <utility>

class Window {
  private:
    SDL_Window *window = nullptr;
    SDL_GLContext context = nullptr; 

  public:
    Window(const std::string& title, int width, int height, SDL_WindowFlags flags);
    ~Window(); 

    //disable copying
    Window(const Window&) = delete;
    auto operator=(const Window&) -> Window& = delete;

    //enable moving
    Window(Window&&) noexcept = default;
    auto operator=(Window&&) noexcept  -> Window& = default;

    [[nodiscard]] auto get_size() const noexcept -> std::pair<int, int>;
    [[nodiscard]] auto get_handle() const noexcept  -> SDL_Window*;
    [[nodiscard]] auto get_context() const noexcept -> SDL_GLContext;
};


#endif // !VOXIDE_WINDOW_HEADER
