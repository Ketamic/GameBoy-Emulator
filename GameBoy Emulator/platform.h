#pragma once

#include "SDL2/include/SDL.h"
#include <stdexcept>

class platform
{
private:
	void CreateWindow();

	SDL_Window* window;
	SDL_Surface* window_surface;
	SDL_Renderer* renderer;

	SDL_Event event;

public:
	void init();
	void destroy();
	void StepSDL();
	SDL_Event* getEvent();
};

