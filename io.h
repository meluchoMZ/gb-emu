/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __IO_H_
#define __IO_H_

#include <stdbool.h>
#include <stdio.h>

/**
 * Shows the guide on how to start the emulator on standard output
 */
void printHelp();

/**
 * Processes the input parameters when starting the emulator
 * @param argc number of input parameters
 * @param argv input parameters
 * @param logFile a pointer to the log file
 * @return the path to the game rom, NULL otherwise
 */
const char * processStartupArguments(int argc, char *argv[], FILE *logFile);

/**
 * Gives a pointer to read the game ROM file
 * @param path the path to the game ROM location
 * @param logFile a pointer to the log file
 */
FILE * loadCartridge(const char *path, FILE *logFile);

/**
 * Closes the game ROM file
 * @param cartridge a FILE pointer to the game ROM file
 * @param logFile a pointer to the log file
 */
void unloadCartridge(FILE *cartridge, FILE *logFile);

#endif // __IO_H_
