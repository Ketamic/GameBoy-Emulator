#include "CPU.h"

#include <fstream>
#include <iostream>
#include <stdio.h>

void CPU::init() {
	printf("initializing...");
	//init_registers();
	init_opcodes();
	init_opcodes16();
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

void CPU::stepCPU() {
	int xyzzy = 0;
	while (true) {
		printf("\n\nOPCODE: 0x%X\n\n", (memory.read(PC)));
		printf("PC: 0x%X	SP: 0x%X\n AF: 0x%X   BC: 0x%X   DE: 0x%X   HL: 0x%X   LY: 0x%X\n", PC, SP, AF, BC, DE, HL, memory.read(0xFF44));
		//printf(" 0xFF44 = %X \n", memory.read(0xFF44));
		/*
		if (memory.read(PC) == 0xCD) {
			
			xyzzy = 0;
		}
		*/
		// If there is a 16-bit opcode then pull from the 16-bit opcode map
		//if (memory.read(PC == 0xCB)) {

		//}
		//else { // If it is not then pull from the normal map
			// Get the Opcode from the Opcode map
			(this->*Opcodes[memory.read(PC)])();
		//}


		//std::cin.get();
		if (xyzzy == 0) { // Run X amount of cycles
			std::cin >> xyzzy;
		}
		--xyzzy;

		
		printf("\nVRAM 0x8010: %X", memory.read(0x8010));
		printf("\nVRAM 0x8020: %X", memory.read(0x8020));

		printf("\nVRAM 0x8190: %X", memory.read(0x8190));
		printf("\nVRAM 0x8191: %X", memory.read(0x8191));
		printf("\nVRAM 0x8192: %X", memory.read(0x8192));
		printf("\nVRAM 0x8193: %X", memory.read(0x8193));
		printf("\nVRAM 0x8194: %X", memory.read(0x8194));
		printf("\nVRAM 0x8195: %X", memory.read(0x8195));
		printf("\nVRAM 0x8196: %X", memory.read(0x8196));
		//this->*(opcode_map.find(0x00)->second)(this);
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

	return;
}
