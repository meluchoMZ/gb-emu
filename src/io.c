/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "io.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void printHelp()
{
	fprintf(stdout, "\tGame Boy emulator\n");
	fprintf(stdout, "\tCopyright: Miguel Blanco Godón\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "\tUsage:\n");
	fprintf(stdout, "\t\t./gb-emu <rom-path>\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "\t\t <rom-path>: path to game ROM\n");
}

const char * processStartupArguments(int argc, char *argv[], FILE *logFile)
{
	if (logFile == NULL) {
		fprintf(stderr, "[IO] Cannot process startup arguments: logFile is NULL\n");
		return NULL;
	}

	if (argc != 2) {
		fprintf(logFile, "[IO] Invalid number of parameters: %d\n", argc);
		return NULL;
	}

	return argv[argc - 1];
}

FILE * loadCartridge(const char *path, FILE *logFile)
{
	if (logFile == NULL) {
		fprintf(stderr, "[IO] Cannot open game ROM: logFile is NULL\n");
		return NULL;
	}

	FILE * cartridge = fopen(path, "r");
	if (cartridge == NULL) {
		fprintf(logFile, "[IO] Cannot open game ROM: %s\n", strerror(errno));
	}

	return cartridge;
}

void unloadCartridge(FILE *cartridge, FILE *logFile)
{
	if (logFile == NULL) {
		fprintf(stderr, "[IO] Cannot close game ROM: logFile is NULL\n");
		return;
	}
	if (fclose(cartridge) != 0) {
		fprintf(logFile, "[IO] Cannot close game ROM: %s\n", strerror(errno));
	}
}
