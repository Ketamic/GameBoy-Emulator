#pragma once
#include "Memory.h"

class PPU
{
public:
	void init(Memory* nmemory);
	void StepPPU(int cycles);
private:
	int CPUCycleAmount;
	Memory* memory;
};