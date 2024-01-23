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

    // Set Background color to white and render
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
}

void platform::SetScreenArray(int i, int j, bool value) {
    ScreenArray[i][j] = value;
}

bool platform::GetScreenArray(int i, int j) {
    return ScreenArray[i][j];
}

void platform::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Draws all of the rectangles from the ScreenArray with the correct color
// Doesn't feel very optmized but i'll look at it later if it becomes an issue
void platform::SetupScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < LCD_WIDTH; ++i) {
        for (int j = 0; j < LCD_HEIGHT; ++j) {
            if (GetScreenArray(i, j) == 1) {
                SDL_Rect* rect = new SDL_Rect({ i, j, 1, 1 });
                SDL_RenderFillRect(renderer, rect);
                delete(rect);
            }
            else {
                throw std::runtime_error("ScreenArray value is out of range");
            }
        }
    }
    
    // This sets the value of RenderedScreenArray to ScreenArray so we know what screen is rendered
    memcpy(RenderedScreenArray, ScreenArray, sizeof(RenderedScreenArray));
}

void platform::StepSDL() {
    SDL_PollEvent(&event);

    // Comparing the two arrays to see if they are equivalent and not to render if nothing has changed
    if (memcmp(RenderedScreenArray, ScreenArray, sizeof(RenderedScreenArray)) != 0) {
        SetupScreen();

        // Set anywhere that wasn't colored white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
}

SDL_Event* platform::getEvent() {
    return &event;
}