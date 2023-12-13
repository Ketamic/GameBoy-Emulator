#include "CPU.h"

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

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
	TODO:
	
	Fix AF register
	Figure out why Half carry flag is being enabled (Could be related to AF flag)
*/

std::string padRegister(uint8_t value) {
	std::stringstream formatted;
	formatted << std::setw(4) << std::setfill('0') << value;
	return formatted.str().c_str();
}

std::string CPU::stepCPU(int log) {

	// starting my timer
	auto start_timer = std::chrono::high_resolution_clock::now();

	// outputting a string so that way main can print or log it to a file
	std::stringstream output;

	// std::hex only works with int so I have to cast my uint8_t to an unsigned int
	output << "\nOPCODE: 0x" << std::hex << unsigned int(memory.read(PC)) << "\n";

	// Adding all of 
	output << "PC: 0x" << padRegister(PC); output << " SP: 0x" << padRegister(SP);
	output << " AF: 0x" << padRegister(PC); output << " BC: 0x" << padRegister(BC);
	output << " DE: 0x" << padRegister(DE); output << " HL: 0x" << padRegister(PC) << "\n";

	output << "CARRY: " << +F.CARRY_FLAG << " HALF-CARRY: " << +F.HALF_CARRY_FLAG << " SUBTRACT: " << +F.SUBTRACT_FLAG << " ZERO: " << +F.ZERO_FLAG << "\n";


	(this->*Opcodes[memory.read(PC)])();

	// Increment Program Counter so the CPU keeps moving foward
	++PC;

	//temporary fix to LY register because gfx aren't setup yet
	memory.write(0xFF44, (memory.read(0xFF44) + 1) % 153);

	auto stop_timer = std::chrono::high_resolution_clock::now();

	std::chrono::microseconds duration_timer = std::chrono::duration_cast<std::chrono::microseconds>(stop_timer - start_timer);
	printf("This step took %d micrsoseconds\n\n", duration_timer.count());

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

	float progress = 0.0;
	const int barwidth = 70;

	// Loading the ROM into memory
	for (int i = 0; i < fileSize; ++i) {
		// Creates a nice progress bar
		printProgress((i + 1.0) / fileSize);

		// actually writes the data to the filebuf
		memory.write(i, fileBuf[i]);
	}

	//std::cin.get();
	delete[]fileBuf;
	fclose(file);   // Almost forgot this 

	std::cout << "\nROM successfully loaded!" << std::endl;

	return;
}
