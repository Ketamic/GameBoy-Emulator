#pragma once
#include "Memory.h"
#include "platform.h"
#include <vector>

class PPU
{
public:
	void init(Memory* nmemory, platform* nplat);
	void StepPPU(int cycles);
private:
	int CPUCycleAmount;
	Memory* memory;
	platform* plat;

	void OutputTiles();
	void OutputTile(int x, int y, int tile_number, int right_offset = 0, int left_offset = 0x8, int bottom_offset = 0, int top_offset = 0x8);
	void RenderScanLine();
};