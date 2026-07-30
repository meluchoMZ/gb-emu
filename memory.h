/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

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

// DMG memory mapped IO registers
// Color Game Boy (CGB) registers will not be implemented yet
#define R_JOYPAD_INPUT       0xFF00
#define R_SERIAL_TRANSFER    0xFF01
#define R_TIMER              0xFF04
#define R_INTERRUPTS         0xFF0F
#define R_AUDIO              0xFF10
#define R_WAVE_PATTERN       0xFF3F
#define R_LCD_CONTROL        0xFF40
#define R_DMA_TRANFER        0xFF46
#define R_BOOT_ROM_MAPPING   0xFF50


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

#endif //__MEMORY_H_
