/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __CARTRIDGE_H
#define __CARTRIDGE_H


#include <stdio.h>
#include <stdint.h>

#define ENTRY_POINT_START 0x0100
#define ENTRY_POINT_END   0x0103

#define NINTENDO_LOGO_START 0x0104
#define NINTENDO_LOGO_END   0x0133

#define TITLE_START 0x0134
#define TITLE_END   0x0143

#define MANUFACTURER_CODE_START 0x013F
#define MANUFACTURER_CODE_END   0x0142

#define CGB_FLAG 0x0143
#define CGB_COLOR_ONLY 0xC0
#define CGB_MONOCHROME 0x80

#define OLD_LICENSEE_VALUE 0x33
#define NEW_LICENSEE_START 0x0144
#define NEW_LICENSEE_END   0x0145

#define SGB_FLAG 0x03

#define CARTRIDGE_TYPE 0x0147
#define CT_ROM_ONLY                       0x00
#define CT_MBC1                           0x01
#define CT_MBC1_RAM                       0x02
#define CT_MBC1_RAM_BATTERY               0x03
#define CT_MBC2                           0x05
#define CT_MBC2_BATTERY                   0x06
#define CT_ROM_RAM                        0x08
#define CT_ROM_RAM_BATTERY                0x09
#define CT_MMM01                          0x0B
#define CT_MMM01_RAM                      0x0C
#define CT_MMM01_RAM_BATTERY              0x0D
#define CT_MBC3_TIMER_BATTERY             0x0F
#define CT_MBC3_TIMER_RAM_BATTERY         0x10
#define CT_MBC3                           0x11
#define CT_MBC3_RAM                       0x12
#define CT_MBC3_RAM_BATTERY               0x13
#define CT_MBC5                           0x19
#define CT_MBC5_RAM                       0x1A
#define CT_MBC5_RAM_BATTERY               0x1B
#define CT_MBC5_RUMBLE                    0x1C
#define CT_MBC5_RUMBLE_RAM                0x1D
#define CT_MBC5_RUMBLE_RAM_BATTERY        0x1E
#define CT_MBC6                           0x20
#define CT_MBC7_SENSOR_RUMBLE_RAM_BATTERY 0x22
#define CT_POCKET_CAMERA                  0xFC
#define CT_BANDAI_TAMA5                   0xFD
#define CT_HUC3                           0xFE
#define CT_HUC1_RAM_BATTERY               0xFF

#define ROM_SIZE 0x0148
#define ROM_SIZE_32_KIB  0x00
#define ROM_SIZE_64_KIB  0x01
#define ROM_SIZE_128_KIB 0x02
#define ROM_SIZE_256_KIB 0x03
#define ROM_SIZE_512_KIB 0x04
#define ROM_SIZE_1_MIB   0X05
#define ROM_SIZE_2_MIB   0X06
#define ROM_SIZE_4_MIB   0X07
#define ROM_SIZE_8_MIB   0X08
#define ROM_SIZE_1_1_MIB 0x52
#define ROM_SIZE_1_2_MIB 0x53
#define ROM_SIZE_1_5_MIB 0x54

#define RAM_SIZE 0x0149
#define RAM_SIZE_NONE    0x00
#define RAM_SIZE_UNUSED  0x01
#define RAM_SIZE_8_KIB   0x02
#define RAM_SIZE_32_KIB  0x03
#define RAM_SIZE_128_KIB 0x04
#define RAM_SIZE_64_KIB  0x05

#define DESTINATION_CODE 0x014A
#define DESTINATION_CODE_JAPAN         0x00
#define DESTINATION_CODE_OVERSEAS_ONLY 0x01

#define OLD_LICENSEE_CODE 0x014B

#define ROM_VERSION_NUMBER 0x014C

#define HEADER_CHECKSUM       0x014D
#define GLOBAL_CHECKSUM_START 0x014E
#define GLOBAL_CHECKSUM_END   0x014F

/**
 * Struct to store the Cartridge header metadata.
 * Each cartridge type changes how the memory works, as some
 * include extra ram/rom data
 */
struct Cartridge
{
	FILE *logFile;
	FILE *romFile;
	// 16 bytes for data + 1 byte for string termination character
	uint8_t title[17];
	uint8_t manufacturerCode[4];
	uint8_t cgbFlag;
	uint16_t newLicenseeCode;
	uint8_t sgbFlag;
	uint8_t cartridgeType;
	uint8_t romSize;
	uint8_t ramSize;
	uint8_t destinationCode;
	uint8_t oldLicenseeCode;
	uint8_t romVersion;
	uint8_t headerChecksum;
	uint16_t globalChecksum;
};

/**
 * Initializes the Cartridge struct with the given log file and path to
 * the desired game rom
 */
bool initCartridge(struct Cartridge *cartridge, FILE *logFile, const char *romFilePath);

/**
 * Writes to the Cartridge.logFile file the metadata information
 * stored on the Cartridge
 */
void printCartridgeInformation(struct Cartridge *cartridge);

/**
 * Frees memory for the cartridge metadata
 */
void finalizeCartridge(struct Cartridge *cartridge);

#endif //__CARTRIDGE_H
