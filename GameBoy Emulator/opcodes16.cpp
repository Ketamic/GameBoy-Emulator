#include "CPU.h"

// Utility function that returns the exact bit asked for, mostly useful for the 16-bit opcodes
std::uint8_t CPU::GetBit(uint8_t reg, int bit) {
	return (reg & (1 << bit)) >> bit;
}

std::uint8_t CPU::SetBit(std::uint8_t value, int bit, int s) {
	return (value & ~(1UL << bit)) | (s << bit);
}

// BIT
void CPU::op_0xCB7C() {
	F.ZERO_FLAG = GetBit(H, 7);
}

// RL
void CPU::op_0xCB11() {
	for (int i = 7; i > 0; --i) {
		SetBit(C, i - 1, GetBit(C, i));
	}
	SetBit(C, 0, F.CARRY_FLAG);
}




void CPU::init_opcodes16() {
	// BIT
	Opcodes16[0x7C] = &CPU::op_0xCB7C;

	//RL
	Opcodes16[0x11] = &CPU::op_0xCB11;
}
