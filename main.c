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

void processingLoop(struct CPU *cpu, struct MMAP *mmap)
{
	while (true) 
	{
		executeCPUInstruction(cpu, mmap);
	}
	fprintf(stderr, "[PC: %02X] - End\n", cpu->PC);
}

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

	//char gameName[16];
	//fprintf(logFile, "Game: ");
	//for (uint16_t offset = 0x0134; offset < 0x0143; ++offset) {
	//	gameName[offset - 0x0134] = mmap.memoryBuffer[offset];
	//}
	//fprintf(logFile, "%s\n", gameName);

	//fprintf(logFile, "Nintendo scrolling logo: ");
	//for (uint16_t offset = 0x0104; offset < 0x0134; ++offset) {
	//	fprintf(logFile, "%02X", mmap.memoryBuffer[offset]);
	//}
	//fprintf(logFile, "\n");

	//while (!proccessInput()) 
	//{
	//	renderFrame(&ppu, gameName);
	//	usleep(16);
	//}
	//processingLoop(&cpu, &mmap);


	finalizePPU(&ppu);

	return EXIT_SUCCESS;
}
