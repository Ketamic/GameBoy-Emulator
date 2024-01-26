#include "PPU.h"

#define LCD_VERT_CYCLES 456
#define LCD_VERT_LINES  154

// Initialization
void PPU::init(Memory* nmemory, platform* nplat) {
	memory = nmemory;
	plat = nplat;
}

// Just spits out all the tiles in VRAM to the screen
// More of a testing feature than a useful one
void PPU::OutputTiles() {
	printf("outputting tiles...");
	for(int k = 0; )
	for (int i = 0; i < 0x10; i += 2) {
		std::uint8_t mem = memory->read(0x8010 + i);
		for (int j = 0; j < 8; ++j) {
			if (((mem >> j) & 1U) == 1) {
				plat->SetScreenArray(8 - j, i / 2, 0xFF000000);
			}
			else {
				plat->SetScreenArray(8 - j, i / 2, 0xFFFFFFFF);
			}
		}
	}
}

// Steps the PPU
void PPU::StepPPU(int cycles) {
	CPUCycleAmount += cycles;
	if (CPUCycleAmount >= LCD_VERT_CYCLES) {
		// After a certain amount of cycles
		memory->write(0xFF44, (memory->read(0xFF44) + 1) % 153);
	}
	OutputTiles();
}