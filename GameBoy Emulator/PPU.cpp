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
	int x_index = 0;
	int y_index = 0;
	int k_index = 1;
	if (memory->read(0x8010) != 0) {
		printf("outputting tiles...\n");
		for (int k = 0x8000; k < 0x9FFF; k += 0x10) {
			printf("K: 0x%X", k);
			for (int i = 0; i < 0x10; i += 2) {
				std::uint8_t mem = memory->read(k + i);
				for (int j = 0; j < 8; ++j) {
					if (((mem >> j) & 1U) == 1) {
						plat->SetScreenArray((8 - j) + x_index, (i / 2) + y_index, 0xFF000000);
					}
					else {
						plat->SetScreenArray((8 - j) + x_index, (i / 2) + y_index, 0xFFFFFFFF);
					}
				}
			}
			if (k_index == (k % 0x8000) / 100) {
				x_index = 0;
				y_index += 9;
				++k_index;
			}
			else {
				x_index += 9;
			}
		}
	}
}

/*
* If the LY Register (0xFF44) is greater than 144 PPU is in vblank
*/

// Steps the PPU
void PPU::StepPPU(int cycles) {
	CPUCycleAmount += cycles;
	if (CPUCycleAmount >= LCD_VERT_CYCLES) {
		// After a certain amount of cycles
		memory->write(0xFF44, (memory->read(0xFF44) + 1) % 153);
		CPUCycleAmount %= LCD_VERT_LINES;
	}
}