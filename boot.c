/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "boot.h"
#include "cartridge.h"
#include "cpu.h"
#include "memory.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

bool loadBootRom(struct MMAP *mmap)
{
	FILE *bootRom = fopen(BIOS_PATH, "r");	
	if (bootRom == NULL) {	
		fprintf(stderr, "[BOOT] Could not open BIOS file: %s\n",
				strerror(errno));
		return false;
	}

	size_t readBytes = fread(mmap->bootRom, 1, BIOS_SIZE, bootRom);
	fclose(bootRom);
	if (readBytes != BIOS_SIZE) {
		fprintf(stderr, "[BOOT] Boot ROM size does not match\n");
		return false;
	}

	return true;
}

/**
 * Read cartridge metadata to obtain cartridge type and fill 
 * the cartridge metadata fields of the struct.
 * This is needed so the power up sequence is able to recognize
 * each cartridge type and loads it to memory accordingly
 */
void readCartridgeMetadata(struct Cartridge *cartridge)
{
	rewind(cartridge->romFile);
	// Game title
	fseek(cartridge->romFile, TITLE_START, SEEK_SET);
	fread(cartridge->title, 1, TITLE_END - TITLE_START + 1, cartridge->romFile);
	// Manufacturer code
	fseek(cartridge->romFile, MANUFACTURER_CODE_START, SEEK_SET);
	fread(cartridge->manufacturerCode, 1, MANUFACTURER_CODE_END - MANUFACTURER_CODE_START + 1, cartridge->romFile);
	// CGB flag
	fseek(cartridge->romFile, CGB_FLAG, SEEK_SET);
	fread(&cartridge->cgbFlag, 1, 1, cartridge->romFile);
	// New licensee code
	fseek(cartridge->romFile, NEW_LICENSEE_START, SEEK_SET);
	fread(&cartridge->newLicenseeCode, 1, 2, cartridge->romFile);
	// SGB flag
	fseek(cartridge->romFile, SGB_FLAG, SEEK_SET);
	fread(&cartridge->sgbFlag, 1, 1, cartridge->romFile);
	// Cartridge type
	fseek(cartridge->romFile, CARTRIDGE_TYPE, SEEK_SET);
	fread(&cartridge->cartridgeType, 1, 1, cartridge->romFile);
	// ROM size
	fseek(cartridge->romFile, ROM_SIZE, SEEK_SET);
	fread(&cartridge->romSize, 1, 1, cartridge->romFile);
	// RAM size
	fseek(cartridge->romFile, RAM_SIZE, SEEK_SET);
	fread(&cartridge->ramSize, 1, 1, cartridge->romFile);
	// Destination code
	fseek(cartridge->romFile, DESTINATION_CODE, SEEK_SET);
	fread(&cartridge->destinationCode, 1, 1, cartridge->romFile);
	// Old licensee code
	fseek(cartridge->romFile, OLD_LICENSEE_CODE, SEEK_SET);
	fread(&cartridge->oldLicenseeCode, 1, 1, cartridge->romFile);
	// ROM version
	fseek(cartridge->romFile, ROM_VERSION_NUMBER, SEEK_SET);
	fread(&cartridge->romVersion, 1, 1, cartridge->romFile);
	// Header checksum 
	fseek(cartridge->romFile, HEADER_CHECKSUM, SEEK_SET);
	fread(&cartridge->headerChecksum, 1, 1, cartridge->romFile);
	// Global checksum
	fseek(cartridge->romFile, GLOBAL_CHECKSUM_START, SEEK_SET);
	fread(&cartridge->globalChecksum, 1, GLOBAL_CHECKSUM_END - GLOBAL_CHECKSUM_START + 1, cartridge->romFile);
}

/**
 * Loads a ROM ONLY cartridge into memory
 */
bool loadRomOnlyCartridgeIntoMemory(struct Cartridge *cartridge, struct MMAP *mmap)
{
	// This type of cartridge do not require baking, so we map it directly from the
	// start of the memory map up to the start of the VRAM data
	rewind(cartridge->romFile);
	size_t readBytes = fread(mmap->ROMBank, 1, VRAM - 1, cartridge->romFile);
	// we check that the read bytes match the size of the cartridge
	return readBytes == 0x7FFF;
}

/**
 * Loads a cartridge into memory taking into account the cartridge type
 */
bool loadCartridgeIntoMemory(struct Cartridge *cartridge, struct MMAP *mmap)
{
	switch (cartridge->cartridgeType)
	{
		case CT_ROM_ONLY:
			return loadRomOnlyCartridgeIntoMemory(cartridge, mmap);
		default:
			fprintf(stderr, "[BOOT] Cartridge type 0x%02hhX is currently not supported\n", cartridge->cartridgeType);
			return false;
	}
}

bool startPowerUpSequence(struct CPU *cpu, struct MMAP *mmap, struct Cartridge *cartridge)
{
	if (!loadBootRom(mmap)) {
		return false;

	}
	readCartridgeMetadata(cartridge);
	printCartridgeInformation(cartridge);
	if (!loadCartridgeIntoMemory(cartridge, mmap)) {
		return false;
	}
	fprintf(stdout, "[BOOT] Loaded cartridge into memory\n");
	return true;
}

