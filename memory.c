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

#define IER    0xFFFF
#define HIRAM  0xFF80
#define EUIO2  0xFF4C
#define IOPORT 0xFF00
#define EUIO   0xFEA0 
#define SAM    0xFE00
#define ERAM   0xE000
#define RAM    0xC000
#define SRAM   0xA000
#define VRAM   0x8000
#define SROM   0x4000
#define ROM    0x0000

#define ROM_SIZE 256

bool loadBootRom(struct MMAP *mmap, FILE *logFile)
{
	FILE *bootRom = fopen("bios/DMG_ROM.bin", "r");	
	if (bootRom == NULL) {	
		fprintf(logFile, "[MEMORY] Could not open BIOS file: %s\n",
				strerror(errno));
		return false;
	}
	// boot ROM is 256B
	uint8_t bootRomBuffer[ROM_SIZE];
	size_t readBytes = fread(&bootRomBuffer, 1, ROM_SIZE, bootRom);
	fclose(bootRom);
	if (readBytes != ROM_SIZE) {
		fprintf(logFile, "[MEMORY] Boot ROM size does not match\n");
		return false;
	}

	for (uint16_t offset = 0x00; offset < ROM_SIZE; ++offset) {
		mmap->bootRom[offset] = bootRomBuffer[offset];
	}

	return true;
}

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
	
	if (!loadBootRom(mmap, logFile)) {
		fprintf(logFile, "[MEMORY] Error loading boot ROM\n");
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

bool loadCartridgeIntoMemory(struct MMAP *mmap, FILE *cartridge)
{
	fprintf(mmap->logFile, "[MMAP] Loading ROM data into memory\n");
	fseek(cartridge, 0, SEEK_END);
	size_t romSize = ftell(cartridge);
	rewind(cartridge);
	// The cartridge size can be variable up to 8MB
	// We use heap memory to avoid possible stack overflows
	uint8_t *romBuffer = malloc(romSize);
	if (romBuffer == NULL) {
		fprintf(mmap->logFile, "[MEMORY] Cannot allocate ROM buffer memory%s\n",
				strerror(errno));
		return false;
	}
	size_t readBytes = fread(romBuffer, 1, romSize, cartridge);
	if (readBytes != romSize) {
		fprintf(mmap->logFile, "[MEMORY] Error reading ROM file\n");
		free(romBuffer);
		return false;
	}
	for (uint16_t offset = ROM; offset < VRAM; ++offset) {
		mmap->memoryBuffer[offset] = romBuffer[offset];
	}
	free(romBuffer);
	fprintf(mmap->logFile, "[MMAP] Loaded 32 kB of data from the cartridge to 0x0000 - 0x7FFF\n");
	return true;
}
