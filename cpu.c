/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "cpu.h"
#include "cpu_instructions.h"

#include <stdbool.h>
#include <stdio.h>

bool initCPU(struct CPU *cpu, FILE *logFile)
{
	if (logFile == NULL) {
		fprintf(stderr, "[CPU] Error initializing CPU: logFile is NULL");
		return false;
	}

	if (cpu == NULL) {
		fprintf(logFile, "[CPU] Error initializing CPU: cpu is NULL");
		return false;
	}

	cpu->logFile = logFile;

	// init Program Counter
	// executable code starts at 0x100
	// Nintendo logo starts at 0x104
	cpu->PC = 0x100;

	// Interrupts are disabled on startup
	cpu->IME = false;

	// init cpu operations
	cpu->instructionSet[0x0][0x0] = &op0x00;
	cpu->instructionSet[0x0][0x3] = &op0x03;
	cpu->instructionSet[0x0][0x8] = &op0x08;
	cpu->instructionSet[0x0][0xB] = &op0x0B;
	cpu->instructionSet[0x0][0xC] = &op0x0C;
	cpu->instructionSet[0x0][0xD] = &op0x0D;
	cpu->instructionSet[0x0][0xE] = &op0x0E;

	cpu->instructionSet[0x1][0x0] = &op0x10;
	cpu->instructionSet[0x1][0x1] = &op0x11;
	cpu->instructionSet[0x1][0xF] = &op0x1F;

	cpu->instructionSet[0x3][0x3] = &op0x33;
	cpu->instructionSet[0x3][0xE] = &op0x3E;

	cpu->instructionSet[0x6][0x3] = &op0x63;
	cpu->instructionSet[0x6][0x6] = &op0x66;
	cpu->instructionSet[0x6][0x7] = &op0x67;
	cpu->instructionSet[0x6][0xE] = &op0x6E;

	cpu->instructionSet[0x7][0x3] = &op0x73;

	cpu->instructionSet[0x8][0x3] = &op0x83;
	cpu->instructionSet[0x8][0x8] = &op0x88;
	cpu->instructionSet[0x8][0x9] = &op0x89;

	cpu->instructionSet[0x9][0x9] = &op0x99;
	cpu->instructionSet[0x9][0xF] = &op0x9F;

	cpu->instructionSet[0xB][0x9] = &op0xB9;
	cpu->instructionSet[0xB][0xB] = &op0xBB;

	cpu->instructionSet[0xC][0xC] = &op0xCC;
	cpu->instructionSet[0xC][0xE] = &op0xCE;

	cpu->instructionSet[0xD][0x9] = &op0xD9;
	cpu->instructionSet[0xD][0xC] = &op0xDC;
	cpu->instructionSet[0xD][0xD] = &op0xDD;

	cpu->instructionSet[0xE][0x6] = &op0xE6;
	cpu->instructionSet[0xE][0xC] = &op0xEC;
	cpu->instructionSet[0xE][0xD] = &op0xED;

	return true;
}

