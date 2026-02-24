/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __CPU_H_
#define __CPU_H_

#include "memory.h"

#include <stdio.h>
#include <stdint.h>

// Need to declare it here to avoid scoping issues in CPU_OP definition
struct CPU;
typedef void (*CPU_OP) (struct CPU *cpu, struct MMAP *mmap);

/**
 * The CPU consist of eight 8-bit registers which can be 
 * extended to four 16-bit registers pairs
 * +-------+--------+
 * | 8-bit |-16 bit |
 * +-------+--------+
 * | A - F |   AF   |
 * | B - C |   BC   |
 * | D - E |   DE   |
 * | H - L |   HL   |
 * +----------------+
 */
struct CPU 
{
	FILE *logFile;
	// PC: Program Counter
	uint16_t PC;
	// SP: Stack Pointer
	uint16_t SP;
	// 8-bit registers
	union
	{
		struct
		{
			struct
			{
				uint8_t _unused : 4;
				uint8_t C : 1;
				uint8_t H : 1;
				uint8_t N : 1;
				uint8_t Z : 1;
			} F;
			uint8_t A;
		};
	}; uint16_t AF;
	union
	{
		struct
		{
			uint8_t C;
			uint8_t B;
		};
	}; uint16_t BC;
	union
	{
		struct
		{
			uint8_t E;
			uint8_t D;
		};
	}; uint16_t DE;
	union
	{
		struct
		{
			uint8_t L;
			uint8_t H;
		};
	}; uint16_t HL;

	// Enable interrupt flag
	bool IME;

	// Function pointer matrix to the instruction set
	CPU_OP instructionSet[16][16];
};

/**
 * Initializes CPU subsystem
 * @param cpu a pointer to a struct CPU instance
 * @param logFile a pointer to the FILE to log the cpu info
 */
bool initCPU(struct CPU *cpu, FILE *logFile);

#endif //__CPU_H_
