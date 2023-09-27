#include "CPU.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip>

void CPU::init() {
	printf("initializing...");
	//init_registers();
	init_opcodes();
	init_opcodes16();

	F.CARRY_FLAG = 0;
	F.HALF_CARRY_FLAG = 0;
	F.SUBTRACT_FLAG = 0;
	F.ZERO_FLAG = 0;
	//memory.init();
}

/*
void printBits(size_t const size, void const* const ptr)
{
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
} */

/*
	TODO:
	
	Fix AF register
	Figure out why Half carry flag is being enabled (Could be related to AF flag)
*/

void CPU::stepCPU() {
	std::string n;
	std::cin >> n;
	bool logging_flag = (n == "log");
	std::ofstream logging_file("opcodes.log");

	int xyzzy = 0;
	while (true) {
		// If the user doesn't want to log, everything will just be printed to the screen
		if (logging_flag == false) {
			printf("\n\nNEXT OPCODE: 0x%X\n\n", (memory.read(PC)));
			printf("PC: 0x%X	SP: 0x%X\n AF: 0x%X   BC: 0x%X   DE: 0x%X   HL: 0x%X   LY: 0x%X\n", PC, SP, AF, BC, DE, HL, memory.read(0xFF44));
			printf("CARRY: %d   HALF-CARRY: %d   SUBTRACT: %d   ZERO: %d\n", F.CARRY_FLAG, F.HALF_CARRY_FLAG, F.SUBTRACT_FLAG, F.ZERO_FLAG);
		} else { // If the user does want to log, it will be logged to opcodes.log
			logging_file << std::hex << "PC: 0x" << PC << " SP: " << SP << " AF: " << AF << " BC: " << BC << " DE: " << DE << " HL: " << HL << "\n";
			logging_file << "CARRY: " << +F.CARRY_FLAG << " HALF-CARRY: " << +F.HALF_CARRY_FLAG << " SUBTRACT: " << +F.SUBTRACT_FLAG << " ZERO: " << +F.ZERO_FLAG << "\n\n";

			logging_file << "HRAM: " << "\n FFFE: " << +memory.read(0xFFFE) << +memory.read(0xFFFD) << "\n FFFC: " << +memory.read(0xFFFC) << +memory.read(0xFFFB) << "\n FFFA: " << +memory.read(0xFFFA) << +memory.read(0xFFF9) << "\n\n";

			logging_file << "OPCODE: " << std::uppercase << std::hex << +memory.read(PC) << "\n";
		}

		(this->*Opcodes[memory.read(PC)])();

		// Run X amount of cycles if the user isn't logging
		if (xyzzy == 0 && logging_flag == false) {
			std::cin >> xyzzy;
		}
		--xyzzy;
		
		// Increment Program Counter so the CPU keeps moving foward
		++PC;

		//temporary fix to LY register because gfx aren't setup yet
		memory.write(0xFF44, (memory.read(0xFF44) + 1) % 153);
	}

}

// Get the size of a file
long getFileSize(FILE* file)
{
	long lCurPos, lEndPos;
	lCurPos = ftell(file);
	fseek(file, 0, 2);
	lEndPos = ftell(file);
	fseek(file, lCurPos, 0);
	return lEndPos;
}

void CPU::loadROM(char const* path)
{
	// An unsigned char can store 1 Bytes (8bits) of data (0-255)
	typedef std::uint8_t BYTE;

	const char* filePath = path;
	BYTE* fileBuf;			// Pointer to our buffered data
	FILE* file = NULL;		// File pointer

	// Open the file in binary mode using the "rb" format string
	// This also checks if the file exists and/or can be opened for reading correctly
	if ((file =	fopen(filePath, "rb")) == NULL)
		std::cout << "Could not open specified file" << std::endl;
	else
		std::cout << "File opened successfully" << std::endl;

	// Get the size of the file in bytes
	long fileSize = getFileSize(file);

	// Allocate space in the buffer for the whole file
	fileBuf = new BYTE[fileSize];

	// Read the file in to the buffer
	fread(fileBuf, fileSize, 1, file);

	/* Now that we have the entire file buffered, we can take a look at some binary infomation
	// Lets take a look in hexadecimal
	for (int i = 0; i < 100; i++)
		printf("%X ", fileBuf[i]); */

	// Loading the ROM into memory
	for (int i = 0; i < fileSize; ++i) {
		memory.write(i, fileBuf[i]);
	}

	//std::cin.get();
	delete[]fileBuf;
	fclose(file);   // Almost forgot this 

	std::cout << "ROM successfully loaded!" << std::endl;

	return;
}
