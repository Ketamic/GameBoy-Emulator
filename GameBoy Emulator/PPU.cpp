#include "PPU.h"

#define LCD_VERT_CYCLES 456
#define LCD_VERT_LINES  154
#define LCD_WIDTH       160
#define LCD_HEIGHT      144

void PPU::init(Memory* nmemory) {
	memory = nmemory;
}

void PPU::StepPPU(int cycles) {
	CPUCycleAmount += cycles;
	if (CPUCycleAmount >= LCD_VERT_CYCLES) {
		// After a certain amount of cycles
		memory->write(0xFF44, (memory->read(0xFF44) + 1) % 153);
	}
}