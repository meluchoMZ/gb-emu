/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "cpu.h"
#include "io.h"
#include "memory.h"
#include "ppu.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
	FILE *logFile = stderr;
	struct CPU cpu;
	struct MMAP mmap;
	struct PPU ppu;

	const char *romLocation = processStartupArguments(argc, argv, logFile);
	if (romLocation == NULL) {
		printHelp();
		return EXIT_FAILURE;
	}

	FILE *cartridge = loadCartridge(romLocation, logFile);
	if (cartridge == NULL) {
		return EXIT_FAILURE;
	}

	if (!initCPU(&cpu, logFile)) {
		unloadCartridge(cartridge, logFile);
		fprintf(logFile, "Could not initialize CPU subsystem. Exiting...\n");
		return EXIT_FAILURE;
	}

	if (!initMemoryMap(&mmap, logFile)) {
		unloadCartridge(cartridge, logFile);
		fprintf(logFile, "Could not initialize MMAP subsystem. Exiting...\n");
		return EXIT_FAILURE;
	}
	
	if (!initPPU(&ppu, logFile)) {
		unloadCartridge(cartridge, logFile);
		fprintf(logFile, "Could not initialize PPU subsystem. Exiting...\n");
		return EXIT_FAILURE;
	}

	if (!loadCartridgeIntoMemory(&mmap, cartridge)) {
		unloadCartridge(cartridge, logFile);
		finalizePPU(&ppu);
		fprintf(logFile, "Could not load cartridge into memory. Exiting...\n");
	}

	char gameName[16];
	fprintf(logFile, "Game: ");
	for (uint16_t offset = 0x0134; offset < 0x0143; ++offset) {
		gameName[offset - 0x0134] = mmap.memoryBuffer[offset];
	}
	fprintf(logFile, "%s\n", gameName);

	fprintf(logFile, "Nintendo scrolling logo: ");
	for (uint16_t offset = 0x0104; offset < 0x0134; ++offset) {
		fprintf(logFile, "%02X", mmap.memoryBuffer[offset]);
	}
	fprintf(logFile, "\n");

	while (!proccessInput()) 
	{
		renderFrame(&ppu, gameName);
		usleep(16);
	}

	finalizePPU(&ppu);
	unloadCartridge(cartridge, logFile);

	return EXIT_SUCCESS;
}
