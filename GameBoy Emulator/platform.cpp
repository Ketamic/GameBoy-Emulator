#include "platform.h"

#define LCD_WIDTH   160
#define LCD_HEIGHT  144

void platform::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n SDL Error: %s", SDL_GetError());
        throw std::logic_error("SDL2 Failed to initialize");
    }

    window = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LCD_WIDTH, LCD_HEIGHT, 0);

    if (!window) {
        printf("Failed to initialize window\n SDL Error: %s", SDL_GetError());
        throw std::logic_error("SDL2 Failed load window");
    }

    window_surface = SDL_GetWindowSurface(window);

    if (!window_surface) {
        printf("Failed to get surface from the window\n SDL Error: %s", SDL_GetError());
        throw std::logic_error("SDL2 failed to get surface from the window");
    }
}