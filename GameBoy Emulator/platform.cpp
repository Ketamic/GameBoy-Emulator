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

    screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, LCD_WIDTH, LCD_HEIGHT);

    if (!screen_texture) {
        printf("Could not create the screen texture: %s", SDL_GetError());
        throw std::logic_error("SDL2 Failed to create the screen texture");
    }

    // Creates a white background to start
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void platform::SetScreenArray(int i, int j, int value) {
    ScreenArray[i][j] = value;
}

int platform::GetScreenArray(int i, int j) {
    return ScreenArray[i][j];
}

void platform::destroy() {
    SDL_DestroyTexture(screen_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Draws all of the rectangles from the ScreenArray with the correct color
// Doesn't feel very optmized but i'll look at it later if it becomes an issue
void platform::SetupScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    uint32_t* texture_buffer = new uint32_t[LCD_WIDTH + LCD_HEIGHT];

    //for (int i = 0; i < LCD_WIDTH; ++i) {
    //    for (int j = 0; j < LCD_HEIGHT; ++j) {
    //        if (GetScreenArray(i, j) == 1) {
    //            SDL_Rect* rect = new SDL_Rect({ i, j, 1, 1 });
    //           SDL_RenderFillRect(renderer, rect);
    //            delete(rect);
    //        }
    //    }
    //}

    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; ++i) {
        if (GetScreenArray(i / LCD_HEIGHT, i % LCD_WIDTH) == 1) {
            texture_buffer[i] = 0xFF000000;
        }
    }

    SDL_UpdateTexture(screen_texture, NULL, texture_buffer, LCD_WIDTH * sizeof(uint32_t));
}

void platform::StepSDL() {
    SDL_PollEvent(&event);

    // Comparing the two arrays to see if they are equivalent and not to render if nothing has changed
    if (memcmp(RenderedScreenArray, ScreenArray, sizeof(RenderedScreenArray)) != 0) {

        // Setting up each pixel for rendering
        SetupScreen();

        // This sets the value of RenderedScreenArray to ScreenArray so we know what screen is rendered
        memcpy(RenderedScreenArray, ScreenArray, sizeof(RenderedScreenArray));

        // Setting ScreenArray to 0 so it can be written to again by the program
        memset(ScreenArray, 0, sizeof(ScreenArray));


        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
}

SDL_Event* platform::getEvent() {
    return &event;
}