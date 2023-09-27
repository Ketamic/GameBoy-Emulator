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
	
	CPU GBCPU = CPU();

	GBCPU.init();

	GBCPU.loadROM("Tetris (World) (Rev 1).gb");
	GBCPU.loadROM("dmg_rom.bin"); // mapping boot rom on top of the game ROM

	GBCPU.stepCPU();
	
	
	return 0;
	
}