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
	void OutputTile(int x, int y, int tile_number);

	std::vector<int> scy_reg;
};