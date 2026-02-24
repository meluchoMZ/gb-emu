/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct MMAP
{
	FILE *logFile;
	// 2 bytes: 2^16
	uint16_t memoryBuffer[65536];
	// pointers to memory mapped locations
	uint16_t *interruptEnableRegister;
	uint16_t *highInternalRAM;
	uint16_t *emptyUnusableForIO2;
	uint16_t *IOPorts;
	uint16_t *emptyUnusableForIO;
	uint16_t *spriteAttribMemory;
	uint16_t *echoRAM;
	uint16_t *internalRAM;
	uint16_t *switchableRAMBank;
	uint16_t *videoRAM;
	uint16_t *switchableROMBank;
	uint16_t *ROMBank;
};

bool initMemoryMap(struct MMAP *mmap, FILE *logFile);

uint8_t readMemory(struct MMAP *mmap, uint16_t address);

void writeMemory(struct MMAP *mmap, uint16_t address, uint8_t data);

#endif //__MEMORY_H_
