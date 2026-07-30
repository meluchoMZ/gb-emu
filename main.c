/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "boot.h"
#include "cartridge.h"
#include "cpu.h"
#include "cpu_instructions.h"
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
	struct Cartridge cartridge;

	const char *romLocation = processStartupArguments(argc, argv, logFile);
	if (romLocation == NULL) {
		printHelp();
		return EXIT_FAILURE;
	}

	if (!initCartridge(&cartridge, logFile, romLocation)) {
		fprintf(logFile, "Could not initialize cartridge data. Exiting...\n");
		return EXIT_FAILURE;
	}

	if (!initCPU(&cpu, logFile)) {
		finalizeCartridge(&cartridge);
		fprintf(logFile, "Could not initialize CPU subsystem. Exiting...\n");
		return EXIT_FAILURE;
	}

	if (!initMemoryMap(&mmap, logFile)) {
		finalizeCartridge(&cartridge);
		fprintf(logFile, "Could not initialize MMAP subsystem. Exiting...\n");
		return EXIT_FAILURE;
	}
	
	if (!initPPU(&ppu, logFile)) {
		finalizeCartridge(&cartridge);
		fprintf(logFile, "Could not initialize PPU subsystem. Exiting...\n");
		return EXIT_FAILURE;
	}

	startPowerUpSequence(&cpu, &mmap, &cartridge);

	char gameTitle[16];
	for (uint8_t i = 0; i < 16; ++i) {
		gameTitle[i] = cartridge.title[i];
	}
	while (!proccessInput())
	{
		executeCPUInstruction(&cpu, &mmap);
		renderFrame(&ppu, gameTitle);
		usleep(16);
	}


	finalizePPU(&ppu);

	return EXIT_SUCCESS;
}
