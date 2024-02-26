#pragma once

#include "SDL2/include/SDL.h"
#include <stdexcept>

#define LCD_WIDTH   160
#define LCD_HEIGHT  144

#define WINDOW_SIZE_MODIFIER 4

class platform
{
private:
	void SetupScreen();

	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Texture* screen_texture;

	SDL_Event event;

	uint32_t ScreenArray[LCD_WIDTH][LCD_HEIGHT];
	uint32_t RenderedScreenArray[LCD_WIDTH][LCD_HEIGHT];

public:
	void init();
	void destroy();

	void StepSDL();
	SDL_Event* getEvent();

	// Getters and Setters for ScreenArray
	void SetScreenArray(int i, int j, uint32_t value);
	uint32_t GetScreenArray(int i, int j);
};

