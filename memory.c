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

	// set initial values for the IO registers
	mmap->memoryBuffer[R_JOYPAD_INPUT] = 0x0F;
	mmap->memoryBuffer[R_BOOT_ROM_MAPPING] = 0X00;
	
	return true;
}

uint8_t readMemory(struct MMAP *mmap, uint16_t address)
{
	// the boot room is a switchable bank
	if (mmap->bootRomEnabled) {
		return mmap->bootRom[address];
	}
	switch (address)
	{
		// Handle reads from IO registers
		case R_JOYPAD_INPUT:
			// temporary return hardcoded no buttons pressed while
			// the joypad support is not developed
			return 0x0F;
		default:
			return mmap->memoryBuffer[address];
	}
}

void writeMemory(struct MMAP *mmap, uint16_t address, uint8_t data)
{
	switch (address)
	{
		// Handle writes to IO registers
		case R_BOOT_ROM_MAPPING:
			mmap->bootRomEnabled = false;
			mmap->memoryBuffer[address] = data;
			return;
		default:
			mmap->memoryBuffer[address] = data;
	}
}

