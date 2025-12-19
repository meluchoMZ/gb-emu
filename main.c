/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "ppu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
	FILE *logFile = stderr;
	struct PPU ppu;
	
	if (initPPU(&ppu, logFile) > 0) {
		return EXIT_FAILURE;
	}

	while (!proccessInput()) 
	{
		renderFrame(&ppu);
		usleep(16);
	}


	finalizePPU(&ppu);

	return EXIT_SUCCESS;
}
