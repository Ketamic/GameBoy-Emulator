#pragma once
#include <cstdint>
#include <unordered_map>
#include "Memory.h"
#include "Flags.h"

class CPU
{
public:
	void init();
	void stepCPU();
	void loadROM(char const* path);

private:
	void init_opcodes();
	void init_opcodes16();

	// Registers
	// This weird formatting allows me to access either just the eight bit register OR the whole 16 bit registers,
	// with any value changes on one affecting the other correctly

	union {
		struct {
			Flags F;
			std::uint8_t A;
		};
		std::uint16_t AF = 0x0000;
	};
	union {
		struct {
			std::uint8_t C;
			std::uint8_t B;
		};
		std::uint16_t BC = 0x0000;
	};
	union {
		struct {
			std::uint8_t E;
			std::uint8_t D;
		};
		std::uint16_t DE = 0x0000;
	};
	union {
		struct {
			std::uint8_t L;
			std::uint8_t H;
		};
		std::uint16_t HL = 0x0000;
	};

	// Stack Pointer - Initialized to the top of the WRAM
	//std::uint16_t SP = 0xE000;
	std::uint16_t SP = 0x0000;
	// Program Counter
	//std::uint16_t PC = 0x0100;
	std::uint16_t PC = 0x0000;

	// Flags
	/*std::uint8_t ZERO_FLAG = 0x00;
	std::uint8_t SUBTRACT_FLAG	 = 0x00;
	std::uint8_t HALF_CARRY_FLAG = 0x00; // Check if there is overflow from the lower nibble to the upper nibble
	std::uint8_t CARRY_FLAG		 = 0x00; */

	// Interupts
	bool HALT = 0;
	bool INTERUPT_MASTER_ENABLE = 0;

	//Memory
	//std::uint8_t ROM_BANK00[0x3FFF] = {};

	//
	//std::uint8_t memory[0xFFFF] = {};
	Memory memory = Memory();

	typedef void (CPU::* Opcode)(void);
	Opcode Opcodes[0xFF + 1];
	Opcode Opcodes16[0xFF + 1];

	// Nice utility functions for memory
	void StackPush(uint16_t value);
	std::uint16_t GetImmediateOperands();
	std::uint8_t GetImmediateOperand();
	std::uint8_t GetBit(uint8_t reg, int shift);
	std::uint8_t SetBit(std::uint8_t value, int bit, int s);

	// OPCODES

	// NOP
	void op_0x00();

	// LD
	void op_0x01();
	void op_0x02();

	// LD
	void op_0x21();
	void op_0x32();

	void op_0x77();

	void op_0xE0();
	void op_0xF0();
	void op_0xFA();
	void op_0x2A();
	void op_0xE2();

	void op_0xEA();

	void op_0x31();

	void op_0x36();

	void op_0x0E();
	void op_0x3E();

	void op_0x40();
	void op_0x50();
	void op_0x60();
	void op_0x70();

	void op_0x78();

	//JR
	void op_0x20();

	void op_0x03();
	void op_0x06();
	void op_0x07();
	void op_0x08();
	void op_0x09();
	void op_0x0A();
	void op_0x0F();
	void op_0x10();
	void op_0x11();
	void op_0x12();
	void op_0x13();

	void op_0xC3();

	// INC
	void op_0x0C();
	void op_0x1C();
	void op_0x2C();
	void op_0x3C();

	void op_0x04();
	void op_0x14();
	void op_0x24();
	void op_0x34();

	// DEC
	void op_0x0D();
	void op_0x1D();
	void op_0x2D();
	void op_0x3D();
	void op_0x05();
	void op_0x15();
	void op_0x25();
	void op_0x35();

	void op_0x0B();
	void op_0x1B();
	void op_0x2B();
	void op_0x3B();

	//OR
	void op_0xB0();
	void op_0xB1();
	void op_0xB2();
	void op_0xB3();
	void op_0xB4();
	void op_0xB5();
	void op_0xB6();
	void op_0xB7();

	// XOR
	void op_0xA8();
	void op_0xA9();
	void op_0xAA();
	void op_0xAB();
	void op_0xAC();
	void op_0xAD();
	void op_0xAE();
	void op_0xAF();

	// CALL
	void op_0xCD();

	// RST
	void op_0xFF(); // RST 7

	// DI
	void op_0xF3();

	// PUSH
	void op_0xF5();

	// CP d8
	void op_0xFE();

	// ------------------ 16-BIT OPCODES ------------------ 
	void op_0xCB();

	// BIT
	void op_0xCB7C();

	// RL
	void op_0xCB11();


};