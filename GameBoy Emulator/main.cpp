//#include <SDL.h>
#include <string>
#include <iostream>

#include "CPU.h"

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

	GBCPU->init();

	GBCPU->loadROM("Tetris (World) (Rev 1).gb");
	GBCPU->loadROM("dmg_rom.bin"); // mapping boot rom on top of the game ROM

	std::ofstream logging_file;

	std::string n;
	printf("Would you like to log to a file? (y/n) ");
	std::cin >> n;
	bool logging_flag = (n == "y");

	if (logging_flag) {
		logging_file.open("opcodes.log");

		if (!logging_file.is_open()) {
			printf("Logging file failed to open!");
			throw std::runtime_error("Logging file failed to open");
		}
	}

	int xyzzy = 0;

	while (true) {

		if (logging_flag) {
			logging_file <<	GBCPU->stepCPU(logging_flag) << std::flush;
		}
		else {
			printf("%s", GBCPU->stepCPU(logging_flag).c_str());
		}

		// This is rarely used now that I have my file output so I'm just going to comment
		// this out until I need it again
		/* Run X amount of cycles if the user isn't logging
		if (xyzzy == 0 && logging_flag == false) {
			std::cin >> xyzzy;
		}
		--xyzzy;
		*/
	}
	
	logging_file.close();
	
	return 0;
	
}