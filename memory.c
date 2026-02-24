/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "memory.h"

#include <stdbool.h>

bool initMemoryMap(struct MMAP *mmap, FILE *logFile)
{
	if (logFile == NULL) {
		fprintf(stderr, "[MEMORY] Error initializing memory: log file is NULL\n");
		return false;
	}

	if (mmap == NULL) {
		fprintf(logFile, "[MEMORY] Error initializing memory: memory map is NULL\n");
		return false;
	}

	mmap->logFile = logFile;
	mmap->interruptEnableRegister = &mmap->memoryBuffer[0xFFFF];
	mmap->highInternalRAM         = &mmap->memoryBuffer[0xFF80];
	mmap->emptyUnusableForIO2     = &mmap->memoryBuffer[0xFF4C];
	mmap->IOPorts                 = &mmap->memoryBuffer[0xFF00];
	mmap->emptyUnusableForIO      = &mmap->memoryBuffer[0xFEA0];
	mmap->spriteAttribMemory      = &mmap->memoryBuffer[0xFE00];
	mmap->echoRAM                 = &mmap->memoryBuffer[0xE000];
	mmap->internalRAM             = &mmap->memoryBuffer[0xC000];
	mmap->switchableRAMBank       = &mmap->memoryBuffer[0xA000];
	mmap->videoRAM                = &mmap->memoryBuffer[0x8000];
	mmap->switchableROMBank       = &mmap->memoryBuffer[0x4000];
	mmap->ROMBank                 = &mmap->memoryBuffer[0x0000];
	
	return true;
}

uint8_t readMemory(struct MMAP *mmap, uint16_t address)
{
	return mmap->memoryBuffer[address];
}

void writeMemory(struct MMAP *mmap, uint16_t address, uint8_t data)
{
	mmap->memoryBuffer[address] = data;
}

