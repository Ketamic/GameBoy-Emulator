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


// LD (HL-), A
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

// LD A, (a16)
void CPU::op_0xFA() {
	A = memory.read(GetImmediateOperands());
}

// LD A, (HL+)
void CPU::op_0x2A() {
	A = memory.read(HL);
	++HL;
}

// LD (C), A
void CPU::op_0xE2() {
	memory.write((0xFF << 8 | C & 0xFF), A);
}

// LD (a16), A - Store the contents of register A in the internal RAM or register specified by the 16-bit immediate operand a16.
void CPU::op_0xEA() {
	memory.write(GetImmediateOperands(), A);
}

//LD SP, d16
void CPU::op_0x31() {
	SP = GetImmediateOperands();
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

//LD B, B - Load the contents of register B into register B.
void CPU::op_0x40() {
	//B = B; // This operation keeps B the same
}

//LD D, B - Load the contents of register B into register B.
void CPU::op_0x50() {
	D = B;
}

//LD H, B - Load the contents of register B into register B.
void CPU::op_0x60() {
	H = B;
}

//LD (HL), B - Store the contents of register B in the memory location specified by register pair HL.
void CPU::op_0x70() {
	memory.write(HL, B);
}

// LD A, B - Load the contents of register B into register A.
void CPU::op_0x78() {
	A = B;
}

// JR

// JR NZ, r8
void CPU::op_0x20() {
	if (F.ZERO_FLAG == 0) {
		std::int8_t s8 = (std::int8_t)GetImmediateOperand();
		//--PC; // keep the ++PC in the immediate operands from changing the PC value
		PC += s8;
		
	}
	else {
		++PC;
	}
}

// INC

// INC C - Increment the contents of C by 1
void CPU::op_0x0C() {
	++C;
	F.ZERO_FLAG = (C == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = ((C & 0x0F) == 0x00);
}

// INC E - Increment the contents of C by 1
void CPU::op_0x1C() {
	++E;
	F.ZERO_FLAG = (E == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = ((E & 0x0F) == 0x00);
}

// INC L - Increment the contents of C by 1
void CPU::op_0x2C() {
	++L;
	F.ZERO_FLAG = (L == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = ((L & 0x0F) == 0x00);
}

// INC A - Increment the contents of C by 1
void CPU::op_0x3C() {
	++A;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = ((A & 0x0F) == 0x00);
}

// INC B
void CPU::op_0x04() {
	++B;
	F.ZERO_FLAG = (B == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = ((B & 0x0F) == 0x00);
}

// INC D
void CPU::op_0x14() {
	++D;
	F.ZERO_FLAG = (D == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = ((D & 0x0F) == 0x00);
}

// INC H
void CPU::op_0x24() {
	++H;
	F.ZERO_FLAG = (H == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = ((H & 0x0F) == 0x00);
}

// INC (HL)
void CPU::op_0x34() {
	memory.write(HL, memory.read(HL) + 1);
	F.ZERO_FLAG = (memory.read(HL) == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = ((memory.read(HL) & 0x0F) == 0x00);
}

// DEC

// DEC C - Decrement the contents of register C by 1
void CPU::op_0x0D() {
	--C;
	F.ZERO_FLAG = (C == 0x00); F.SUBTRACT_FLAG = 1; F.HALF_CARRY_FLAG = ((C & 0x0F) == 0x00);
}

// DEC E - Decrement the contents of register E by 1
void CPU::op_0x1D() {
	--E;
	F.ZERO_FLAG = (E == 0x00); F.SUBTRACT_FLAG = 1; F.HALF_CARRY_FLAG = ((E & 0x0F) == 0x00);
}

// DEC L - Decrement the contents of register L by 1
void CPU::op_0x2D() {
	--L;
	F.ZERO_FLAG = (L == 0x00); F.SUBTRACT_FLAG = 1; F.HALF_CARRY_FLAG = ((L & 0x0F) == 0x00);
}

// DEC A - Increment the contents of C by 1
void CPU::op_0x3D() {
	--A;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 1; F.HALF_CARRY_FLAG = ((A & 0x0F) == 0x00);
}

// DEC B - Decrement the contents of register B by 1
void CPU::op_0x05() {
	--B;
	F.ZERO_FLAG = (B == 0x00); F.SUBTRACT_FLAG = 1; F.HALF_CARRY_FLAG = ((B & 0x0F) == 0x00);
}

// DEC D - Decrement the contents of register D by 1
void CPU::op_0x15() {
	--D;
	F.ZERO_FLAG = (D == 0x00); F.SUBTRACT_FLAG = 1; F.HALF_CARRY_FLAG = ((D & 0x0F) == 0x00);
}

// DEC H - Decrement the contents of register H by 1
void CPU::op_0x25() {
	--H;
	F.ZERO_FLAG = (H == 0x00); F.SUBTRACT_FLAG = 1; F.HALF_CARRY_FLAG = ((H & 0x0F) == 0x00);
}

// DEC (HL) - Decrement the contents of memory specified by register pair HL by 1.
void CPU::op_0x35() {
	memory.write(HL, memory.read(HL) - 1);
	F.ZERO_FLAG = (memory.read(HL) == 0x00); F.SUBTRACT_FLAG = 1; F.HALF_CARRY_FLAG = ((memory.read(HL) & 0x0F) == 0x00);
}

void CPU::op_0x0B() {
	--BC;
}

void CPU::op_0x1B() {
	--DE;
}

void CPU::op_0x2B() {
	--HL;
}

void CPU::op_0x3B() {
	--SP;
}

// OR

// OR B
void CPU::op_0xB0() {
	A = A | B;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// OR C
void CPU::op_0xB1() {
	A = A | C;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// OR D
void CPU::op_0xB2() {
	A = A | D;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// OR E
void CPU::op_0xB3() {
	A = A | E;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// OR H
void CPU::op_0xB4() {
	A = A | H;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// OR L
void CPU::op_0xB5() {
	A = A | L;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// OR (HL)
void CPU::op_0xB6() {
	A = A | memory.read(HL);
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// OR A
void CPU::op_0xB7() {
	//A = A | A; // This operation won't ever actually change A
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}
// XOR

// XOR B
void CPU::op_0xA8() {
	A = A ^ B;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// XOR C
void CPU::op_0xA9() {
	A = A ^ C;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// XOR D
void CPU::op_0xAA() {
	A = A ^ D;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// XOR E
void CPU::op_0xAB() {
	A = A ^ E;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// XOR H
void CPU::op_0xAC() {
	A = A ^ H;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// XOR L
void CPU::op_0xAD() {
	A = A ^ L;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// XOR (HL)
void CPU::op_0xAE() {
	A = A ^ HL;
	F.ZERO_FLAG = (A == 0x00); F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
}

// XOR A
void CPU::op_0xAF() {
	A = 0x00;
	F.ZERO_FLAG = 1; F.SUBTRACT_FLAG = 0; F.HALF_CARRY_FLAG = 0; F.CARRY_FLAG = 0;
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

// CALL

// CALL a16
void CPU::op_0xCD() {
	uint16_t imm = GetImmediateOperands();
	--SP;	
	memory.write(--SP, PC >> 8);
	memory.write(SP, PC & 0xFF);
	PC = imm - 1; // For some reason one is being added onto the imm, i have zero idea why as of right now
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
	F.ZERO_FLAG = (NN == 0x00);
	F.SUBTRACT_FLAG = 1;
	F.HALF_CARRY_FLAG = (A ^ d8 ^ NN) & 0x10 ? 1 : 0;
	F.CARRY_FLAG = (NN & 0xFF00) ? 1 : 0;
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
	Opcodes[0xFA] = &CPU::op_0xFA;
	Opcodes[0x2A] = &CPU::op_0x2A;
	Opcodes[0xE2] = &CPU::op_0xE2;

	Opcodes[0x78] = &CPU::op_0x78;

	Opcodes[0xEA] = &CPU::op_0xEA;

	Opcodes[0x31] = &CPU::op_0x31;

	Opcodes[0x36] = &CPU::op_0x36;

	Opcodes[0xC3] = &CPU::op_0xC3;

	Opcodes[0x40] = &CPU::op_0x40;
	Opcodes[0x50] = &CPU::op_0x50;
	Opcodes[0x60] = &CPU::op_0x60;
	Opcodes[0x70] = &CPU::op_0x70;

	// JR
	Opcodes[0x20] = &CPU::op_0x20;

	// INC
	Opcodes[0x0C] = &CPU::op_0x0C;
	Opcodes[0x1C] = &CPU::op_0x1C;
	Opcodes[0x2C] = &CPU::op_0x2C;
	Opcodes[0x3C] = &CPU::op_0x3C;

	Opcodes[0x04] = &CPU::op_0x04;
	Opcodes[0x14] = &CPU::op_0x14;
	Opcodes[0x24] = &CPU::op_0x24;
	Opcodes[0x34] = &CPU::op_0x34;
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

	Opcodes[0x0B] = &CPU::op_0x0B;
	Opcodes[0x0B] = &CPU::op_0x1B;
	Opcodes[0x0B] = &CPU::op_0x2B;
	Opcodes[0x0B] = &CPU::op_0x3B;

	Opcodes[0xF3] = &CPU::op_0xF3;

	Opcodes[0xCD] = &CPU::op_0xCD;

	Opcodes[0xFF] = &CPU::op_0xFF;

	// OR
	Opcodes[0xB0] = &CPU::op_0xB0;
	Opcodes[0xB1] = &CPU::op_0xB1;
	Opcodes[0xB2] = &CPU::op_0xB2;
	Opcodes[0xB3] = &CPU::op_0xB3;
	Opcodes[0xB4] = &CPU::op_0xB4;
	Opcodes[0xB5] = &CPU::op_0xB5;
	Opcodes[0xB6] = &CPU::op_0xB6;
	Opcodes[0xB7] = &CPU::op_0xB7;

	//PUSH

	Opcodes[0xF5] = &CPU::op_0xF5;

	// CD
	Opcodes[0xFE] = &CPU::op_0xFE;
}