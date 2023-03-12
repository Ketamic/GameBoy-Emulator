#pragma once

// An implementation of register F that allows me to access the individual bits within it, AKA the different flags
struct Flags {
	unsigned b0 : 1, b1 : 1, b2 : 1, b3 : 1, CARRY_FLAG : 1, HALF_CARRY_FLAG : 1, SUBTRACT_FLAG : 1, ZERO_FLAG : 1;
};