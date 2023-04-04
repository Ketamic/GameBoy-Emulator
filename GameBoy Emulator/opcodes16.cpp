#include "CPU.h"

// Utility function that returns the exact bit asked for, mostly useful for the 16-bit opcodes
std::uint8_t CPU::GetBit(uint8_t number, int bit) {
	return (number >> bit) & 1U;
}

std::uint8_t CPU::SetBit(std::uint8_t number, int n, int x) {
	return number ^ (-x ^ number) & (1UL << n);
}

// BIT
void CPU::op_0xCB7C() {
	F.ZERO_FLAG = GetBit(H, 7);
}

// RL
void CPU::op_0xCB11() {
	uint8_t bit7 = (C, 7);
	C <<= 1;
	C = SetBit(C, 0, F.CARRY_FLAG);
	F.ZERO_FLAG = (C == 0x00); 
	F.SUBTRACT_FLAG = 0;
	F.HALF_CARRY_FLAG = 0;
	F.CARRY_FLAG = bit7;
}




void CPU::init_opcodes16() {
	// BIT
	Opcodes16[0x7C] = &CPU::op_0xCB7C;

	//RL
	Opcodes16[0x11] = &CPU::op_0xCB11;
}
