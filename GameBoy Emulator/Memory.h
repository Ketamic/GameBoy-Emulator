#pragma once
#include <stdint.h>
class Memory
{
private:
	//0000 - 3FFF
	uint8_t ROM_0[0xD000]; // From Cartridge
	//4000 - 7FFF
	uint8_t ROM_1[0xD000]; // From cartridge, switchable bank via mapper (if any) -- UNIMPLIMENTED

	//8000 - 9FFF
	uint8_t VRAM[0x2000]; // Video RAM

	//A000 - BFFF
	uint8_t XRAM[0x2000]; // From cartridge, switchable bank if any -- UNIMPLIMENTED

	//C000 - CFFF
	uint8_t WRAM_0[0x1000]; // Work RAM
	//D000 - DFFF
	uint8_t WRAM_1[0x1000]; // Also Work RAM

	//E000 - FDFF
	uint8_t ECHO_RAM[0x1E00]; // Nintendo says use of this area is prohibited -- UNIMPLIMENTED

	//FE00 - FE9F
	uint8_t OAM[0x00A0]; // Sprite attribute table (OAM)

	//FEA0 - FEFF
	uint8_t UNUSEABLE[0x0060]; // Nintendo says use of this area is prohibited

	//FF00 - FF7F
	uint8_t IO[0x0080];

	//FF80 - FFFE
	uint8_t HRAM[0x007F]; // High RAM

	//FFFF - FFFF
	uint8_t IE; // Interupt Enable Register

public:
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);
};

