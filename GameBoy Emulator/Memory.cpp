#include "Memory.h"

// Initialize values in memory that would usually be set by the boot ROM

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
		if (address <= 0xFF43) {
			return IO[(0xFF7F - address)];
		}
		else if (address == 0xFF44) {
			return LY;
		}
		else {
			return IO[(0xFF7F - address)];
		}
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
		if (address <= 0xFF43) {
			IO[(0xFF7F - address)] = value;
		}
		else if (address == 0xFF44) {
			LY = value;
		}
		else {
			IO[(0xFF7F - address)] = value;
		}
	}
	else if (address <= 0xFFFE) {
		HRAM[0xFFFE - address] = value;
	}
	else if (address <= 0xFFFF) {
		IE = value;
	}
}
