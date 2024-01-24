#include "PPU.h"

#define LCD_VERT_CYCLES 456
#define LCD_VERT_LINES  154

// Initialization
void PPU::init(Memory* nmemory, platform* nplat) {
	memory = nmemory;
	plat = nplat;
}

// Just spits out all the tiles in VRAM to the screen
// More of a cool feature than a useful one
void PPU::OutputTiles() {
	for (int i = 0x8000; i < 0xA000; i += 2) {
		uint8_t mem = memory->read(i);
		for (int j = 0; j < 8; ++j) {
			//plat->SetScreenArray(i, j, ((mem >> j) & 1U));
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
}