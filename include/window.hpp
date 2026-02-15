#pragma once
#ifndef VOXIDE_WINDOW_HEADER

#include <SDL3/SDL.h>
#include <string>
#include <utility>

class Window {
  private:
    SDL_Window *window = nullptr;
    
  public:
    Window(int width, int height, const std::string& title, SDL_WindowFlags flags);
    ~Window(); 

    //disable copying
    Window(const Window&) = delete;
    auto operator=(const Window&) -> Window& = delete;

    //enable moving
    Window(Window&&) noexcept = default;
    auto operator=(Window&&) noexcept  -> Window& = default;


    [[nodiscard]] auto get_size() const noexcept -> std::pair<int, int>;

    [[nodiscard]] auto handle() const noexcept  -> SDL_Window*;
};


#endif // !VOXIDE_WINDOW_HEADER
