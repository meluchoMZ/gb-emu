/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct MMAP
{
	FILE *logFile;
	uint8_t bootRom[256];
	// 2 bytes: 2^16 addresses
	uint8_t memoryBuffer[65536];
	// pointers to memory mapped locations
	uint8_t *interruptEnableRegister;
	uint8_t *highInternalRAM;
	uint8_t *emptyUnusableForIO2;
	uint8_t *IOPorts;
	uint8_t *emptyUnusableForIO;
	uint8_t *spriteAttribMemory;
	uint8_t *echoRAM;
	uint8_t *internalRAM;
	uint8_t *switchableRAMBank;
	uint8_t *videoRAM;
	uint8_t *switchableROMBank;
	uint8_t *ROMBank;

	bool bootRomEnabled;
};

/**
 * Initializes an MMAP structure
 * @param mmap a pointer to a MMAP structure
 * @param logFile a pointer to the log file
 * @return true if the MMAP has been correctly initialized, false otherwise
 */
bool initMemoryMap(struct MMAP *mmap, FILE *logFile);

/**
 * Reads a byte of data from the memory map
 * @param mmap a pointer to a MMAP structure
 * @param address a 2 byte address to access the data
 * @return a byte of data
 */
uint8_t readMemory(struct MMAP *mmap, uint16_t address);

/**
 * Writes a byte of data in the memory map
 * @param mmap a pointer to a MMAP structure
 * @param address a 2 byte adress to where to write the data
 * @param data a byte of data to write in the memory at the given address
 */ 
void writeMemory(struct MMAP *mmap, uint16_t address, uint8_t data);

/**
 * Loads the cartridge data into the Game Boy memory
 * @param mmap a pointer to a MMAP structure
 * @param cartridge a poitner to the ROM file
 * @return true if the load was successfull, false otherwise
 */
bool loadCartridgeIntoMemory(struct MMAP *mmap, FILE *cartridge);

#endif //__MEMORY_H_
