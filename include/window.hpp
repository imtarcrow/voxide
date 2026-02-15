#pragma once
#ifndef VOXIDE_WINDOW_HEADER

#include <SDL3/SDL.h>
#include <string>

class Window {
  private:
    SDL_Window *window = nullptr;
    
  public:
    Window(int width, int height, std::string title);
    ~Window(); 

    [[nodiscard]] auto handle() const noexcept  -> SDL_Window*;
};


#endif // !VOXIDE_WINDOW_HEADER
