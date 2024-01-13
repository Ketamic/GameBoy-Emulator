#pragma once

#include "SDL2/include/SDL.h"
#include <stdexcept>

class platform
{
private:
	void CreateWindow();

	SDL_Window* window;
	SDL_Surface* window_surface;

public:
	void init();
};

