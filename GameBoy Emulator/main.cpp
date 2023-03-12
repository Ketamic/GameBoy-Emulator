//#include <SDL.h>
#include <string>
#include <iostream>

#include "CPU.h"

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

int main(int argc, char* argv[]) {
	
	//std::uint16_t comb = (0xFF << 8| 0x44);
	//printf("0x%X", comb);

	/*
	uint16_t a = 0b1111111100000000;
	uint8_t msb = (a >> 8);
	uint8_t lsb = a & ((1 << 8) - 1);

	printBits(sizeof(msb), &msb); printf("\n"); printBits(sizeof(lsb), &lsb);

	uint16_t cmbnd = lsb | (msb) << 8;

	printBits(sizeof(cmbnd), &cmbnd); */
	/*
	std::uint8_t a = 0xFC;
	std::int8_t sign = (std::int8_t)a;
	
	std::cout << "a: " << unsigned(a) << std::endl << "signed: " << signed(sign) << std::endl;
	*/
	//printf("MSB: %s \nLSB: %s\n", std::to_string(msb).c_str(), std::to_string(lsb).c_str());

	printf("\n\n\n\n\n");
	CPU GBCPU = CPU();

	GBCPU.init();

	//GBCPU.loadROM("C:\\Users\\Cedar\\Downloads\\Tetris (World) (Rev 1)\\Tetris (World) (Rev 1).gb");
	GBCPU.loadROM("C:\\Users\\Cedar\\Downloads\\Tetris (World) (Rev 1).gb");
	GBCPU.loadROM("C:\\Users\\Cedar\\Downloads\\dmg_rom.bin");

	GBCPU.stepCPU();

	return 0;

}