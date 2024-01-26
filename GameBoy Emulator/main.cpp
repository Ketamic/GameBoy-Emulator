#include <string>
#include <iostream>
#include "SDL2/include/SDL.h"

#include "CPU.h"
#include "platform.h"

// Debugging function
void printBits(size_t const size, void const* const ptr)
{
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
}

// Utility function that returns the exact bit asked for, mostly useful for the 16-bit opcodes
std::uint8_t GetBit(uint8_t number, int bit) {
	return number << bit;
}

std::uint8_t SetBit(std::uint8_t number, int n, int x) {
	return number ^ (-x ^ number) & (1UL << n);
}

int main(int argc, char* argv[]) {

	CPU* GBCPU = new CPU();
	platform* plat = new platform();

	GBCPU->init(plat);

	GBCPU->loadROM("Tetris (World) (Rev 1).gb");
	GBCPU->loadROM("dmg_rom.bin"); // mapping boot rom on top of the game ROM

	std::ofstream logging_file;

	std::string logging_type;
	printf("How you like to log the results? (l/p/n) ");
	std::cin >> logging_type;

	if (logging_type == "l") {
		logging_file.open("opcodes.log");

		if (!logging_file.is_open()) {
			printf("Logging file failed to open!");
			throw std::runtime_error("Logging file failed to open");
		}
	}

	// Initializing the SDL2 window
	plat->init();
	bool quit = false;
	SDL_Event* event;

	while (!quit) {
		plat->StepSDL();
		event = plat->getEvent();

		if (event->type == SDL_QUIT) {
			quit = true;
		}

		if (logging_type == "l") {
			logging_file <<	GBCPU->stepCPU(logging_type) << std::flush;
		}
		else if(logging_type == "p") {
			printf("%s", GBCPU->stepCPU(logging_type).c_str());
		}
		else {
			GBCPU->stepCPU(logging_type);
		}
	}
	
	// shutting down
	plat->destroy();

	delete(GBCPU);
	delete(plat);

	logging_file.close();
	
	return 0;
	
}