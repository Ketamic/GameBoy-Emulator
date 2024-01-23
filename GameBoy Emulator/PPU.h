#pragma once
#include "Memory.h"
#include "platform.h"

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
};