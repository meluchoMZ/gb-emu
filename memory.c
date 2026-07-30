/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "memory.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	mmap->interruptEnableRegister = &mmap->memoryBuffer[IER];
	mmap->highInternalRAM         = &mmap->memoryBuffer[HIRAM];
	mmap->emptyUnusableForIO2     = &mmap->memoryBuffer[EUIO2];
	mmap->IOPorts                 = &mmap->memoryBuffer[IOPORT];
	mmap->emptyUnusableForIO      = &mmap->memoryBuffer[EUIO];
	mmap->spriteAttribMemory      = &mmap->memoryBuffer[SAM];
	mmap->echoRAM                 = &mmap->memoryBuffer[ERAM];
	mmap->internalRAM             = &mmap->memoryBuffer[RAM];
	mmap->switchableRAMBank       = &mmap->memoryBuffer[SRAM];
	mmap->videoRAM                = &mmap->memoryBuffer[VRAM];
	mmap->switchableROMBank       = &mmap->memoryBuffer[SROM];
	mmap->ROMBank                 = &mmap->memoryBuffer[ROM];

	mmap->bootRomEnabled = true;
	
	return true;
}

uint8_t readMemory(struct MMAP *mmap, uint16_t address)
{
	// the boot room is a switchable bank
	if (address < 0x0100 && mmap->bootRomEnabled) {
		return mmap->bootRom[address];
	}
	return mmap->memoryBuffer[address];
}

void writeMemory(struct MMAP *mmap, uint16_t address, uint8_t data)
{
	mmap->memoryBuffer[address] = data;
}

