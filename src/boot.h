/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __BOOT_H
#define __BOOT_H

#include "peripherals/cartridge.h"
#include "soc/memory.h"

#include <stdbool.h>

#define BIOS_SIZE 256
#define BIOS_PATH "bios/DMG_ROM.bin"

/**
 * Load cartridge metadata
 * Load cartridge data into memory
 * Execute the BIOS (scrolling Nintendo Logo)
 */
bool startPowerUpSequence(struct MMAP *mmap, struct Cartridge *cartridge);

#endif //__BOOT_H
