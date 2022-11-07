#include "CPU.h"

void CPU::StackPush(uint16_t value) {
	--SP;
	memory.write(SP, (value >> 8)); // MSB
	--SP;
	memory.write(SP, value & ((1 << 8) - 1)); // LSB
}

std::uint16_t CPU::GetImmediateOperands() {
	//	 LSB			   MSB
	PC += 2; // Keeps from reading immedate operand once the opcode is done
	return memory.read(PC - 1) | (memory.read(PC) << 8);
}

std::uint8_t CPU::GetImmediateOperand() {
	++PC; // Keeps from reading immedate operand once the opcode is done
	return memory.read(PC);
}

// NOP - Advances program counter by 1
void CPU::op_0x00() {
	// This does nothing
}

void CPU::op_0x01() {


	//return NULL;
}

// LD

// LD HL - Load the 2 bytes of immediate data into register pair HL.
void CPU::op_0x21() {
	HL = GetImmediateOperands();
}

void CPU::op_0x32() {
	memory.write(HL, A);
	--HL;
}

// LD, (a8) A - Store the contents of register A in the internal RAM, port register, or mode register at the address in the range 0xFF00-0xFFFF specified by the 8-bit immediate operand a8.
void CPU::op_0xE0() {
	memory.write(0xFF << 8 | (GetImmediateOperand() & 0xFF), A);
}

// LD, A (a8) - Load into register A the contents of the internal RAM, port register, or mode register at the address in the range 0xFF00-0xFFFF specified by the 8-bit immediate operand a8.
void CPU::op_0xF0() {
	//printf("\n\n\n 0x%X \n\n\n", (0xFF << 8 | GetImmediateOperand())); PC -= 2;
	A = memory.read(0xFF << 8 | ( GetImmediateOperand() & 0xFF));
}

//LD (HL), d8
void CPU::op_0x36() {
	memory.write(HL, GetImmediateOperand());
}

//LD, C - Load the 8-bit immediate operand d8 into register C.
void CPU::op_0x0E() {
	C = GetImmediateOperand();
}

void CPU::op_0x3E() {
	A = GetImmediateOperand();
}

//LD, B - Load the 8-bit immediate operand d8 into register B.
void CPU::op_0x06() {
	B = GetImmediateOperand();
}

// JR

// JR NZ, r8
void CPU::op_0x20() {
	if (ZERO_FLAG == 0) {
		PC += (std::int8_t)GetImmediateOperand();
	}
	else {
		++PC;
	}
}

// INC

// INC C - Increment the contents of C by 1
void CPU::op_0x0C() {
	++C;
	ZERO_FLAG = (C == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = ((C & 0x0F) == 0x00);
}

// INC E - Increment the contents of C by 1
void CPU::op_0x1C() {
	++E;
	ZERO_FLAG = (E == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = ((E & 0x0F) == 0x00);
}

// INC L - Increment the contents of C by 1
void CPU::op_0x2C() {
	++L;
	ZERO_FLAG = (L == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = ((L & 0x0F) == 0x00);
}

// INC A - Increment the contents of C by 1
void CPU::op_0x3C() {
	++A;
	ZERO_FLAG = (A == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = ((A & 0x0F) == 0x00);
}

// DEC

// DEC C - Decrement the contents of register C by 1
void CPU::op_0x0D() {
	--C;
	ZERO_FLAG = (C == 0x00); SUBTRACT_FLAG = 1; HALF_CARRY_FLAG = ((C & 0x0F) == 0x00);
}

// DEC E - Decrement the contents of register E by 1
void CPU::op_0x1D() {
	--E;
	ZERO_FLAG = (E == 0x00); SUBTRACT_FLAG = 1; HALF_CARRY_FLAG = ((E & 0x0F) == 0x00);
}

// DEC L - Decrement the contents of register L by 1
void CPU::op_0x2D() {
	--L;
	ZERO_FLAG = (L == 0x00); SUBTRACT_FLAG = 1; HALF_CARRY_FLAG = ((L & 0x0F) == 0x00);
}

// DEC A - Increment the contents of C by 1
void CPU::op_0x3D() {
	--A;
	ZERO_FLAG = (A == 0x00); SUBTRACT_FLAG = 1; HALF_CARRY_FLAG = ((A & 0x0F) == 0x00);
}

// DEC B - Decrement the contents of register B by 1
void CPU::op_0x05() {
	--B;
	ZERO_FLAG = (B == 0x00); SUBTRACT_FLAG = 1; HALF_CARRY_FLAG = ((B & 0x0F) == 0x00);
}

// DEC D - Decrement the contents of register D by 1
void CPU::op_0x15() {
	--D;
	ZERO_FLAG = (D == 0x00); SUBTRACT_FLAG = 1; HALF_CARRY_FLAG = ((D & 0x0F) == 0x00);
}

// DEC H - Decrement the contents of register H by 1
void CPU::op_0x25() {
	--H;
	ZERO_FLAG = (H == 0x00); SUBTRACT_FLAG = 1; HALF_CARRY_FLAG = ((H & 0x0F) == 0x00);
}

// DEC (HL) - Decrement the contents of memory specified by register pair HL by 1.
void CPU::op_0x35() {
	memory.write(HL, HL - 1);
	ZERO_FLAG = (memory.read(HL) == 0x00); SUBTRACT_FLAG = 1; HALF_CARRY_FLAG = ((memory.read(HL) & 0x0F) == 0x00);
}


// XOR

// XOR B
void CPU::op_0xA8() {
	A = A ^ B;
	ZERO_FLAG = (A == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = 0; CARRY_FLAG = 0;
}

// XOR C
void CPU::op_0xA9() {
	A = A ^ C;
	ZERO_FLAG = (A == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = 0; CARRY_FLAG = 0;
}

// XOR D
void CPU::op_0xAA() {
	A = A ^ D;
	ZERO_FLAG = (A == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = 0; CARRY_FLAG = 0;
}

// XOR E
void CPU::op_0xAB() {
	A = A ^ E;
	ZERO_FLAG = (A == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = 0; CARRY_FLAG = 0;
}

// XOR H
void CPU::op_0xAC() {
	A = A ^ H;
	ZERO_FLAG = (A == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = 0; CARRY_FLAG = 0;
}

// XOR L
void CPU::op_0xAD() {
	A = A ^ L;
	ZERO_FLAG = (A == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = 0; CARRY_FLAG = 0;
}

// XOR (HL)
void CPU::op_0xAE() {
	A = A ^ HL;
	ZERO_FLAG = (A == 0x00); SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = 0; CARRY_FLAG = 0;
}

// XOR A
void CPU::op_0xAF() {
	A = 0x00;
	ZERO_FLAG = 1; SUBTRACT_FLAG = 0; HALF_CARRY_FLAG = 0; CARRY_FLAG = 0;
}

// JP

//JP nn - Unconditional jump to the absolute address specified by the 16-bit operand
void CPU::op_0xC3() {
	PC = GetImmediateOperands();
	--PC; // We want the location that was jumped to to be executed, so this keeps the PC the same when it is incremented later
}

// DI - Disables interrupt handling by setting IME=0 and cancelling any scheduled effects of the EI instruction if any.
void CPU::op_0xF3() {
	INTERUPT_MASTER_ENABLE = 0;
}

// RST - Unconditional function call to the absolute fixed address defined by the opcode

void CPU::op_0xFF() {
	StackPush(PC);
	PC = 0x0038;
}

// PUSH

// PUSH AF
void CPU::op_0xF5() {
	StackPush(AF);
}

// CP

// CP d8 - Compare the contents of register A and the contents of the 8-bit immediate operand d8 by calculating A - d8, and set the Z flag if they are equal.
void CPU::op_0xFE() {
	std::uint8_t d8 = GetImmediateOperand();
	std::uint8_t NN = (A - d8);
	ZERO_FLAG = (NN == 0x00);
	SUBTRACT_FLAG = 1;
	HALF_CARRY_FLAG = (A ^ d8 ^ NN) & 0x10 ? 1 : 0;
	CARRY_FLAG = (NN & 0xFF00) ? 1 : 0;
}

uint8_t Opcode_Cycles[0x100] = {
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
	4,12, 8, 8, 4, 4, 8, 4,20, 8, 8, 8, 4, 4, 8, 4,    // 0x00
	4,12, 8, 8, 4, 4, 8, 4, 8, 8, 8, 8, 4, 4, 8, 4,    // 0x10
	8,12, 8, 8, 4, 4, 8, 4, 8, 8, 8, 8, 4, 4, 8, 4,    // 0x20
	8,12, 8, 8,12,12,12, 4, 8, 8, 8, 8, 4, 4, 8, 4,    // 0x30
	4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x40
	4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x50
	4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x60
	8, 8, 8, 8, 8, 8, 4, 8, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x70
	4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x80
	4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x90
	4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0xA0
	4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0xB0
	8,12,12,12,12,16, 8,32, 8, 8,12, 8,12,12, 8,32,    // 0xC0
	8,12,12, 0,12,16, 8,32, 8, 8,12, 0,12, 0, 8,32,    // 0xD0
	12,12, 8, 0, 0,16, 8,32,16, 4,16, 0, 0, 0, 8,32,    // 0xE0
	12,12, 8, 4, 0,16, 8,32,12, 8,16, 4, 0, 0, 8,32     // 0xF0
};

// Loading all of the opcodes into the Opcode map
void CPU::init_opcodes() {
	Opcodes[0x00] = &CPU::op_0x00;
	Opcodes[0x01] = &CPU::op_0x01;
	Opcodes[0x0C] = &CPU::op_0x0C;

	// LD
	Opcodes[0x21] = &CPU::op_0x21;
	Opcodes[0x32] = &CPU::op_0x32;
	Opcodes[0x0E] = &CPU::op_0x0E;
	Opcodes[0x3E] = &CPU::op_0x3E;
	Opcodes[0x06] = &CPU::op_0x06;

	Opcodes[0xE0] = &CPU::op_0xE0;
	Opcodes[0xF0] = &CPU::op_0xF0;

	Opcodes[0xC3] = &CPU::op_0xC3;

	// JR
	Opcodes[0x20] = &CPU::op_0x20;

	// INC
	Opcodes[0x0C] = &CPU::op_0x0C;
	Opcodes[0x1C] = &CPU::op_0x1C;
	Opcodes[0x2C] = &CPU::op_0x2C;
	Opcodes[0x3C] = &CPU::op_0x3C;
	/*Opcodes[0x05] = &CPU::op_0x05;		Yet to be implemented
	Opcodes[0x15] = &CPU::op_0x15;
	Opcodes[0x25] = &CPU::op_0x25;
	Opcodes[0x35] = &CPU::op_0x35; */


	// DEC
	Opcodes[0x0D] = &CPU::op_0x0D;
	Opcodes[0x1D] = &CPU::op_0x1D;
	Opcodes[0x2D] = &CPU::op_0x2D;
	Opcodes[0x3D] = &CPU::op_0x3D;
	Opcodes[0x05] = &CPU::op_0x05;
	Opcodes[0x15] = &CPU::op_0x15;
	Opcodes[0x25] = &CPU::op_0x25;
	Opcodes[0x35] = &CPU::op_0x35;

	Opcodes[0xA8] = &CPU::op_0xA8;
	Opcodes[0xA9] = &CPU::op_0xA9;
	Opcodes[0xAA] = &CPU::op_0xAA;
	Opcodes[0xAB] = &CPU::op_0xAB;
	Opcodes[0xAC] = &CPU::op_0xAC;
	Opcodes[0xAD] = &CPU::op_0xAD;
	Opcodes[0xAE] = &CPU::op_0xAE;
	Opcodes[0xAF] = &CPU::op_0xAF;

	Opcodes[0xF3] = &CPU::op_0xF3;

	Opcodes[0xFF] = &CPU::op_0xFF;

	//PUSH

	Opcodes[0xF5] = &CPU::op_0xF5;

	// CD
	Opcodes[0xFE] = &CPU::op_0xFE;
}