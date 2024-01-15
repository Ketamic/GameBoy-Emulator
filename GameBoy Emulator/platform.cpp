#include "platform.h"

void platform::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
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
}

void platform::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Draws all of the rectangles from the ScreenArray with the correct color
// Doesn't feel very optmized but i'll look at it later if it becomes an issue
void platform::SetupScreen() {
    for (int i = 0; i < LCD_WIDTH; ++i) {
        for (int j = 0; j < LCD_HEIGHT; ++j) {
            if (ScreenArray[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            else if(ScreenArray[i][j] == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                throw std::runtime_error("ScreenArray value is out of range");
            }

            SDL_RenderFillRect(renderer, new SDL_Rect({ i, j, 1, 1 }));
        }
    }
}

void platform::StepSDL() {
    SDL_PollEvent(&event);

    SetupScreen();
    
    // Set Background color to white and render
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

SDL_Event* platform::getEvent() {
    return &event;
}