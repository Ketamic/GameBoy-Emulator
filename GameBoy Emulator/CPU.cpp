#include "CPU.h"

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void CPU::init(platform* plat) {
	printf("initializing...\n\n");
	//init_registers();
	init_opcodes();
	init_opcodes16();

	ppu.init(&memory, plat);

	F.CARRY_FLAG = 0;
	F.HALF_CARRY_FLAG = 0;
	F.SUBTRACT_FLAG = 0;
	F.ZERO_FLAG = 0;
}

// Utility function that returns the exact bit asked for, mostly useful for the 16-bit opcodes
std::uint8_t CPU::GetBit(std::uint8_t number, int bit) {
	return (number >> bit) & 1U;
}

std::uint8_t CPU::SetBit(std::uint8_t number, int n, int x) {
	return number ^ (-x ^ number) & (1UL << n);
}

// These two functions are obscenely slow - I haven't quite figured out how to fix it yet
std::string padRegister(std::uint16_t value) {
	std::stringstream formatted;
	formatted << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << value;
	return formatted.str();
}

std::string padMemory(std::uint8_t value) {
	std::stringstream formatted;
	formatted << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << value;
	return formatted.str();
}

uint8_t Opcode_Cycles[0x100] = {
	//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		4, 12,8, 8, 4, 4, 8, 4, 20,8, 8, 8, 4, 4, 8, 4,    // 0x00
		4, 12,8, 8, 4, 4, 8, 4, 8, 8, 8, 8, 4, 4, 8, 4,    // 0x10
		8, 12,8, 8, 4, 4, 8, 4, 8, 8, 8, 8, 4, 4, 8, 4,    // 0x20
		8, 12,8, 8, 12,12,12,4, 8, 8, 8, 8, 4, 4, 8, 4,    // 0x30
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x40
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x50
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x60
		8, 8, 8, 8, 8, 8, 4, 8, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x70
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x80
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0x90
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0xA0
		4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,    // 0xB0
		8, 12,12,12,12,16,8,32, 8, 8, 12,8,12,12, 8,32,    // 0xC0
		8, 12,12, 0,12,16,8,32, 8, 8, 12,0,12, 0, 8,32,    // 0xD0
		12,12,8, 0, 0, 16,8,32,16, 4, 16,0, 0, 0, 8,32,    // 0xE0
		12,12,8, 4, 0, 16,8,32,12, 8, 16,4, 0, 0, 8,32     // 0xF0
};

std::string CPU::stepCPU(std::string log) {

	printf("LCDC Bit 4: %d", GetBit(memory.read(0xFF40), 4));

	// starting my timer
	std::chrono::steady_clock::time_point start_timer = std::chrono::high_resolution_clock::now();

	// outputting a string so that way main can print or log it to a file
	std::stringstream output;

	// If that flag isn't a logging flag then all this will be ignored and a blank string returned
	if (log != "n") {
		// std::hex only works with int so I have to cast my uint8_t to an unsigned int
		output << "\nOPCODE: 0x" << std::uppercase << std::hex << unsigned int(memory.read(PC)) << "\n";

		// The + seen near the variables casts them to an int 

		// adding part of the HRAM block to the output
		//output << "HRAM:\n0xFFFE: " << std::uppercase << std::hex << +memory.read(0xFFFF) << +memory.read(0xFFFE);
		//output << "\n0xFFFC: " << +memory.read(0xFFFD) << +memory.read(0xFFFC);
		//output << "\n0xFFFA: " << +memory.read(0xFFFB) << +memory.read(0xFFFA) << "\n";

		// adding part of the VRAM block to the output
		output << "VRAM:\n0x8010: " << std::uppercase << std::hex << +memory.read(0x8011) << +memory.read(0x8010);
		output << "\n0x8012: " << +memory.read(0x8013) << +memory.read(0x8012);
		output << "\n0x8014: " << +memory.read(0x8015) << +memory.read(0x8014) << "\n";

		// Adding all of the registers to the output
		output << "PC: 0x" << padRegister(PC); output << "  SP: 0x" << padRegister(SP); output << "  LY: 0x" << padRegister(memory.read(0xFF44));
		output << "\nAF: 0x" << padRegister(AF); output << "  BC: 0x" << padRegister(BC);
		output << "  DE: 0x" << padRegister(DE); output << "  HL: 0x" << padRegister(HL) << "\n";

		output << "CARRY: " << +F.CARRY_FLAG << " HALF-CARRY: " << +F.HALF_CARRY_FLAG << " SUBTRACT: " << +F.SUBTRACT_FLAG << " ZERO: " << +F.ZERO_FLAG << "\n";
	}


	// I need to have a copy of the PC 
	std::uint16_t PC_copy = PC;

	// Running opcode from table
	(this->*Opcodes[memory.read(PC)])();

	// Running PPU
	ppu.StepPPU(Opcode_Cycles[PC_copy]);

	// Increment Program Counter so the CPU keeps moving foward
	++PC;

	// Counting how long each step took
	printf("This step took %d micrsoseconds\n\n", (int)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_timer).count());

	return output.str();
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

// Print an amount of progress as a nice looking bar
void printProgress(double percentage) {
	int val = (int)(percentage * 100);
	int lpad = (int)(percentage * PBWIDTH);
	int rpad = PBWIDTH - lpad;
	printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
	fflush(stdout);
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
	if ((file = fopen(filePath, "rb")) == NULL) {
		std::cout << "Could not open specified file" << std::endl;
		return;
	}
	
	std::cout << "File opened successfully" << std::endl;

	// Get the size of the file in bytes
	long fileSize = getFileSize(file);

	// Allocate space in the buffer for the whole file
	fileBuf = new BYTE[fileSize];

	// Read the file in to the buffer
	fread(fileBuf, fileSize, 1, file);

	// Loading the ROM into memory
	for (int i = 0; i < fileSize; ++i) {
		// Creates a nice progress bar
		printProgress((i + 1.0) / fileSize);

		// actually writes the data to the filebuf
		memory.write(i, fileBuf[i]);
	}

	delete[]fileBuf;
	fclose(file); 

	printf("\nROM successfully loaded!\n\n");

	return;
}
