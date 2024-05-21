#include "PPU.h"

#define LCD_VERT_CYCLES 456
#define LCD_VERT_LINES  154

// Initialization
void PPU::init(Memory* nmemory, platform* nplat) {
	memory = nmemory;
	plat = nplat;

}

void PPU::OutputTile(int x, int y, int tile_number) {
	for (int i = 0; i < 0x10; i += 2) {
		// 0x8800 addressing method
		//std::uint8_t mem = memory->read(0x9000 + (tile_number * 0x10));

		// 0x8000 addressing method
		std::uint8_t mem = memory->read(0x8000 + (tile_number * 0x10) + i);

		for (int j = right_offset; j < left_offset; ++j) {
			if (((mem >> j) & 1U) == 1) {
				plat->SetScreenArray((8 - j) + x, (i / 2) + y, 0xFF000000);
			}
			else {
				plat->SetScreenArray((8 - j) + x, (i / 2) + y, 0xFFFFFFFF);
			}
		}
	}
}

// Just spits out all the tiles in VRAM to the screen
// More of a testing feature than a useful one
void PPU::OutputTiles() {
	int x_index = 0;
	int y_index = 0;
	int k_index = 1;
	if (memory->read(0x8010) != 0) {
		printf("outputting tiles...\n");
		for (int k = 0x8000; k < 0x9FFF; k += 0x10) {
			printf("K: 0x%X", k);
			for (int i = 0; i < 0x10; i += 2) {
				std::uint8_t mem = memory->read(k + i);
				for (int j = 0; j < 8; ++j) {
					if (((mem >> j) & 1U) == 1) {
						plat->SetScreenArray((8 - j) + x_index, (i / 2) + y_index, 0xFF000000);
					}
					else {
						plat->SetScreenArray((8 - j) + x_index, (i / 2) + y_index, 0xFFFFFFFF);
					}
				}
			}
			if (k_index == (k % 0x8000) / 100) {
				x_index = 0;
				y_index += 9;
				++k_index;
			}
			else {
				x_index += 9;
			}
		}
	}
}

/*
* If the LY Register (0xFF44) is greater than 144 PPU is in vblank
*/

// Steps the PPU
void PPU::StepPPU(int cycles) {

	// 7 x 32

	
	for (int i = 0; i < 0x12; ++i) {
		OutputTile(20 + (i * 8), 20, memory->read(0x9800 + (8*32) + i), 4);
	} 
	for (int i = 0; i < 0x10; ++i) {
		OutputTile(20 + (i * 8), 28, memory->read(0x9800 + (9*32) + i), 4);
	}
	

	// Have to start at -1 becuase % 0 is always zero and we want to be on row
	// 0 when we start drawing to the screen
	int y = -1;

	// Reading the Background Map onto the LCD
	// Wrapping the screen does not work currently
	for (int i = 0; i < (LCD_WIDTH * LCD_HEIGHT) / 64; ++i) {
		if ((LCD_WIDTH / 8) % 8 == 0) {
			++y;
		}

		//printf("\nx: %d\ny: %d\nTile Number: %X", i * 8, y * 8, memory->read(0x9800 + i + (((memory->read(0xFF42) + 143) % 256) * 32)));

		/*
		* SCY = 0xFF42
		* SCX = 0xFF43
		* Equation for bottom coordinate is (SCY + 143) % 256
		* Equation for right coordinate is (SCX + 159) % 256
		* 
		* LCD height is 144
		* LCD width is 160
		* 
		* The LCD can fit 20 x 18 tiles
		* 
		* Background Map is 32 x 32
		* Each Tile is 8x8
		*/

		/*
		* EXAMPLE:
		* 
		* SCY = 56
		* Getting y-offset
		* (SCY + 143) & 256 = 199
		* Figuring out which row of the table to read from:
		* 199 / 32 = 6.21875
		* 199 >> 5
		* Truncate 6.21875 to 6
		* 6 rows up
		* 199 % 32 % 8 = 7
		* 7 Pixel offset
		* 
		*/


		//OutputTile((i * 8) % LCD_WIDTH, y * 8, );
	}

	printf("\nFF42: %X\n Tile_Number 12: 0x%X\n", memory->read(0xFF42), memory->read(0x9800 + (8 * 32) + 5));
	//printf("\nFF42: %X\n Tile_Number 12: %X\n", memory->read(0xFF42), memory->read(0x9800 + 12 + (((memory->read(0xFF42) + 143) % 256))));


	CPUCycleAmount += cycles;
	if (CPUCycleAmount >= LCD_VERT_CYCLES) {
		// After a certain amount of cycles the LY increases or wraps back to zero
		memory->write(0xFF44, (memory->read(0xFF44) + 1) % LCD_VERT_LINES);
		CPUCycleAmount -= LCD_VERT_LINES;
	}
}