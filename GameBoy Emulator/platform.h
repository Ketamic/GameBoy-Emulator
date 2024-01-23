#pragma once

#include "SDL2/include/SDL.h"
#include <stdexcept>

#define LCD_WIDTH   160
#define LCD_HEIGHT  144

class platform
{
private:
	void SetupScreen();

	SDL_Window* window;
	SDL_Surface* window_surface;
	SDL_Renderer* renderer;

	SDL_Event event;

	bool ScreenArray[LCD_WIDTH][LCD_HEIGHT] = { 0 };
	bool RenderedScreenArray[LCD_WIDTH][LCD_HEIGHT] = { 0 };

public:
	void init();
	void destroy();

	void StepSDL();
	SDL_Event* getEvent();

	// Getters and Setters for ScreenArray
	void SetScreenArray(int i, int j, bool value);
	bool GetScreenArray(int i, int j);

};

