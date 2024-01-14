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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Could not create a renderer: %s", SDL_GetError());
        throw std::logic_error("SDL2 Failed to create renderer");
    }

    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);
}

void platform::StepSDL() {
    SDL_PollEvent(&event);
    SDL_RenderPresent(renderer);
}

SDL_Event* platform::getEvent() {
    return &event;
}