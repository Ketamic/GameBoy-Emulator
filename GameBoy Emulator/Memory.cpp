#include "Memory.h"

// Initializing function for memory that tries to link the registers to their 
// respective spots in memory to make it more readable
void Memory::init() {
	LY = &IO[0x003B];
}

// Reads a value from memory at the specified address
uint8_t Memory::read(uint16_t address) {

	if (address < 0x3FFF) {
		return ROM_0[address];
	} 
	else if (address <= 0x7FFF) {
		return ROM_1[(0x7FFF - address)];
	} 
	else if (address <= 0x9FFF) {
		return VRAM[(0x9FFF - address)];
	} 
	else if (address <= 0xBFFF) {
		return XRAM[(0xBFFF - address)]; 
	} 
	else if (address <= 0xCFFF) {
		return WRAM_0[(0xCFFF - address)];
	}
	else if (address <= 0xDFFF) {
		return WRAM_1[(0xDFFF - address)];
	}
	else if (address <= 0xFDFF) {
		return ECHO_RAM[(0xFDFF - address)];
	}
	else if (address <= 0xFE9F) {
		return OAM[(0xFE9F - address)];
	} 
	else if (address <= 0xFEFF) {
		return 0x00; // This shouldn't ever happen but its just here for consistency
	} 
	else if (address <= 0xFF7F) {
		return IO[(0xFF7F - address)];
	}
	else if (address <= 0xFFFE) {
		return HRAM[0xFFFE - address];
	}
	else if (address <= 0xFFFF) {
		return IE;
	}

	// No address should be outside of these bounds
	return NULL;
}

void Memory::write(uint16_t address, uint8_t value) {
	//printf("Writing 0x%X to 0x%X\n", value, address);
	if (address < 0x3FFF) {
		ROM_0[address] = value;
	}
	else if (address <= 0x7FFF) {
		ROM_1[(0x7FFF - address)] = value;
	}
	else if (address <= 0x9FFF) {
		VRAM[(0x9FFF - address)] = value;
	}
	else if (address <= 0xBFFF) {
		XRAM[(0xBFFF - address)] = value;
	}
	else if (address <= 0xCFFF) {
		WRAM_0[(0xCFFF - address)] = value;
	}
	else if (address <= 0xDFFF) {
		WRAM_1[(0xDFFF - address)] = value;
	}
	else if (address <= 0xFDFF) {
		ECHO_RAM[(0xFDFF - address)] = value;
	}
	else if (address <= 0xFE9F) {
		OAM[(0xFE9F - address)] = value;
	}
	else if (address <= 0xFEFF) {
		return; // This shouldn't ever happen but its just here for consistency -- writing to it does nothing
	}
	else if (address <= 0xFF7F) {
		IO[(0xFF7F - address)] = value;
	}
	else if (address <= 0xFFFE) {
		HRAM[0xFFFE - address] = value;
	}
	else if (address <= 0xFFFF) {
		IE = value;
	}
}

void Memory::DumpMemory(std::string path) {
	printf("Dumping Memory...");
	std::ofstream mem_file;
	mem_file.open(path);
	if (!mem_file.is_open()) {
		printf("Memory Dump file failed to open!");
		throw std::runtime_error("Memory Dump file failed to open");
	}

	printf("%X%X", VRAM[10], VRAM[11]);

	for (int i = 0x1FFE; i >= 0; i -= 2) {
		//mem_file << i << ": \n";
		mem_file << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << 0x8000 + i << ": ";
		mem_file << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << +VRAM[0x1FFF - i + 1];
		mem_file << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << +VRAM[0x1FFF - i] << "\n";
	}
	mem_file.close();
}