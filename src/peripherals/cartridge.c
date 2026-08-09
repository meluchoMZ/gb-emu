/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "cartridge.h"

#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define UNMATCHED_ERROR "--- [ERROR] ---\0"

bool initCartridge(struct Cartridge *cartridge, FILE *logFile, const char *romFilePath)
{
	if (logFile == NULL) {
		fprintf(stderr, "[CARTRIDGE] Error initializing cartridge metadata: logFile is NULL\n");
		return false;
	}

	cartridge->logFile = logFile;
	
	if (romFilePath == NULL) {
		fprintf(logFile, "[CARTRIDGE] Error initializing cartridge: romFilePath is NULL\n");
		return false;
	}

	cartridge->romFile = fopen(romFilePath, "r");
	if (cartridge->romFile == NULL) {
		fprintf(cartridge->logFile, "[CARTRIDGE] Cannot open game rom file: %s\n", strerror(errno));
		return false;
	}

	// purge the title data
	for (int i = 0; i < 17; ++i) {
		cartridge->title[i] = '\0';
	}
	return true;
}

void finalizeCartridge(struct Cartridge *cartridge)
{
	if (cartridge == NULL) {
		return;
	}
	if (cartridge->romFile != NULL) {
		fclose(cartridge->romFile);
		cartridge->romFile = NULL;
	}
}

char * cgbFlagToString(uint8_t cgbFlag)
{
	printf("%x\n", cgbFlag);
	switch (cgbFlag)
	{
		case CGB_MONOCHROME:
			return "Monochrome compatible\0";
		case CGB_COLOR_ONLY:
			return "Game Boy Color only\0";
		default:
			return "Monochrome only\0";
	}
}

char * cartridgeTypeToString(uint8_t cartridgeSize)
{
	switch (cartridgeSize)
	{
		case CT_ROM_ONLY:
			return "ROM ONLY\0";
		case CT_MBC1:
			return "MBC1\0";
		case CT_MBC1_RAM:
			return "MBC1+RAM\0";
		case CT_MBC1_RAM_BATTERY:
			return "MBC1+RAM+BATTERY\0";
		case CT_MBC2:
			return "MBC2\0";
		case CT_MBC2_BATTERY:
			return "MBC2+BATTERY\0";
		case CT_ROM_RAM:
			return "ROM+RAM\0";
		case CT_ROM_RAM_BATTERY:
			return "ROM+RAM+BATTERY\0";
		case CT_MMM01:
			return "MMM01\0";
		case CT_MMM01_RAM:
			return "MMM01+RAM\0";
		case CT_MMM01_RAM_BATTERY:
			return "MMM01+RAM+BATTERY\0";
		case CT_MBC3_TIMER_BATTERY:
			return "MBC3+TIMER+BATTERY\0";
		case CT_MBC3_TIMER_RAM_BATTERY:
			return "MBC3+TIMER+RAM+BATTERY\0";
		case CT_MBC3:
			return "MBC3\0";
		case CT_MBC3_RAM:
			return "MBC3+RAM\0";
		case CT_MBC3_RAM_BATTERY:
			return "MBC3+RAM+BATTERY\0";
		case CT_MBC5:
			return "MBC5\0";
		case CT_MBC5_RAM:
			return "MBC5+RAM\0";
		case CT_MBC5_RAM_BATTERY:
			return "MBC5+RAM+BATTERY\0";
		case CT_MBC5_RUMBLE:
			return "MBC5+RUMBLE\0";
		case CT_MBC5_RUMBLE_RAM:
			return "MBC5+RUMBLE+RAM\0";
		case CT_MBC5_RUMBLE_RAM_BATTERY:
			return "MBC5+RUMBLE+RAM+BATTERY\0";
		case CT_MBC6:
			return "MBC6\0";
		case CT_MBC7_SENSOR_RUMBLE_RAM_BATTERY:
			return "MBC7+SENSOR+RUMBLE+RAM+BATTERY\0";
		case CT_POCKET_CAMERA:
			return "POCKET CAMERA\0";
		case CT_BANDAI_TAMA5:
			return "BANDAI TAMA5\0";
		case CT_HUC3:
			return "HuC3\0";
		case CT_HUC1_RAM_BATTERY:
			return "HuC1+RAM+BATTERY\0";
		default:
			return UNMATCHED_ERROR;
	}
}

char * romSizeToString(uint8_t romSize)
{
	switch (romSize)
	{
		case ROM_SIZE_32_KIB:
			return "32 KiB\0";
		case ROM_SIZE_64_KIB:
			return "64 KiB\0";
		case ROM_SIZE_128_KIB:
			return "128 KiB\0";
		case ROM_SIZE_256_KIB:
			return "256 KiB\0";
		case ROM_SIZE_512_KIB:
			return "512 KiB\0";
		case ROM_SIZE_1_MIB:
			return "1 MiB\0";
		case ROM_SIZE_2_MIB:
			return "2 MiB\0";
		case ROM_SIZE_4_MIB:
			return "4 MiB\0";
		case ROM_SIZE_8_MIB:
			return "8 MiB\0";
		case ROM_SIZE_1_1_MIB:
			return "1.1 MiB\0";
		case ROM_SIZE_1_2_MIB:
			return "1.2 MiB\0";
		case ROM_SIZE_1_5_MIB:
			return "1.5 MiB\0";
		default:
			return UNMATCHED_ERROR;
	}
}

char * ramSizeToString(uint8_t ramSize)
{
	switch (ramSize)
	{
		case RAM_SIZE_NONE:
			return "0\0";
		case RAM_SIZE_UNUSED:
			return "Unused\0";
		case RAM_SIZE_8_KIB:
			return "8 KiB\0";
		case RAM_SIZE_32_KIB:
			return "32 KiB\0";
		case RAM_SIZE_128_KIB:
			return "128 KiB\0";
		case RAM_SIZE_64_KIB:
			return "64 KiB\0";
		default:
			return UNMATCHED_ERROR;
	}
}

char * destinationCodeToString(uint8_t destinationCode)
{
	switch (destinationCode)
	{
		case DESTINATION_CODE_JAPAN:
			return "Japan\0";
		case DESTINATION_CODE_OVERSEAS_ONLY:
			return "Overseas only\0";
		default:
			return UNMATCHED_ERROR;
	}
}

char * getOldLicenseeCode(uint8_t code)
{
	switch (code)
	{
		case 0x00:
			return "NONE\0";
		case 0x01:
		case 0x31:
			return "Nintendo\0";
		case 0x08:
		case 0x38:
			return "Capcom\0";
		case 0x09:
			return "HOT-B\0";
		case 0x0A:
		case 0xE0:
			return "Jaleco\0";
		case 0x0B:
			return "Coconuts Japan\0";
		case 0x0C:
		case 0x6E:
			return "Elite Systems\0";
		case 0x13:
		case 0x69:
			return "EA (Electronic Arts)\0";
		case 0x18:
			return "Hudson Soft\0";
		case 0x19:
			return "ITC Entertainment\0";
		case 0x1A:
			return "Yanoman\0";
		case 0x1D:
			return "Japan Clary\0";
		case 0x1F:
		case 0x4A:
		case 0x61:
			return "Virgin Games Ltd.\0";
		case 0x24:
			return "PCM Complete\0";
		case 0x25:
			return "San-X\0";
		case 0x28:
			return "Kemco\0";
		case 0x29:
			return "SETA Corporation\0";
		case 0x30:
		case 0x70:
			return "Infogrames\0";
		case 0x32:
		case 0xA2:
		case 0xB2:
			return "Bandai\0";
		case 0x33:
			return "-\0";
		case 0x34:
		case 0xA4:
			return "Konami\0";
		case 0x35:
			return "HectorSoft\0";
		case 0x39:
		case 0x9D:
		case 0xD9:
			return "Banpresto\0";
		case 0x3C:
			return "Enternainment Interactive (stub)\0";
		case 0x3E:
			return "Gremlin\0";
		case 0x41:
			return "UbiSoft\0";
		case 0x42:
		case 0xEB:
			return "Atlus\0";
		case 0x44:
		case 0x4D:
			return "Malibu Interactive\0";
		case 0x46:
		case 0xCF:
			return "Angel\0";
		case 0x47:
			return "Spectrum HoloByte\0";
		case 0x49:
			return "Irem\0";
		case 0x4F:
			return "U.S. Gold\0";
		case 0x50:
			return "Absolute\0";
		case 0x51:
			return "Acclaim Entertainment\0";
		case 0x52:
			return "Activision\0";
		case 0x53:
			return "Sammy USA Corporation\0";
		case 0x54:
			return "GameTek\0";
		case 0x55:
			return "Park Place\0";
		case 0x56:
		case 0xDB:
		case 0xFF:
			return "LJN\0";
		case 0x57:
			return "Matchbox\0";
		case 0x59:
			return "Milton Bradley Company\0";
		case 0x5A:
			return "Mindscape\0";
		case 0x5B:
			return "Romstar\0";
		case 0x5C:
			return "Naxat Soft\0";
		case 0x5D:
			return "Tradewest\0";
		case 0x60:
			return "Tidus Interactive\0";
		case 0x67:
			return "Ocean Software\0";
		case 0x6F:
			return "Electro Brain\0";
		case 0x71:
			return "Interplay Entertainment\0";
		case 0x72:
			return "Broderbund\0";
		case 0x73:
			return "Sculptured Software\0";
		case 0x75:
			return "The Sales Curve Limited\0";
		case 0x78:
			return "THQ\0";
		case 0x79:
			return "Accolade\0";
		case 0x7A:
			return "Triffix Entertainment\0";
		case 0x7C:
			return "MicroProse\0";
		case 0x7F:
		case 0x97:
			return "Kemco\0";
		case 0x80:
			return "Misawa Entertainment\0";
		case 0x83:
			return "LOCZ G.\0";
		case 0x86:
			return "Tokuma Shoten\0";
		case 0x8B:
			return "Bullet-Proof Software\0";
		case 0x8C:
			return "Vic Tokai Corp.\0";
		case 0x8E:
			return "Ape Inc.\0";
		case 0x8F:
			return "I'Max\0";
		case 0x91:
			return "Chunsoft Co.\0";
		case 0x92:
			return "Video System\0";
		case 0x93:
			return "Tsubaraya Productions\0";
		case 0x95:
			return "Varie\0";
		case 0x96:
			return "Yonezawa/S'Pal\0";
		case 0x99:
			return "Arc\0";
		case 0x9A:
			return "Nihon Bussan\0";
		case 0x9B:
			return "Tecmo\0";
		case 0x9C:
			return "Imagineer\0";
		case 0x9F:
			return "Nova\0";
		case 0xA1:
			return "Hori Electric\0";
		case 0xA6:
			return "Kawada\0";
		case 0xA7:
			return "Takara\0";
		case 0xA9:
			return "Technos Japan\0";
		case 0xAA:
			return "Broderbund\0";
		case 0xAC:
			return "Toei Animation\0";
		case 0xAD:
			return "Toho\0";
		case 0xAF:
			return "Namco\0";
		case 0xB0:
			return "Acclaim Entertainment\0";
		case 0xB1:
			return "ASCII Corporation\0";
		case 0xB4:
			return "Square Enix\0";
		case 0xB6:
			return "HAL Laboratory\0";
		case 0xB7:
			return "SNK\0";
		case 0xB9:
		case 0xCE:
			return "Pony Canyon\0";
		case 0xBA:
			return "Culture Brain\0";
		case 0xBB:
			return "Sunsoft\0";
		case 0xBD:
			return "Sony Imagesoft\0";
		case 0xBF:
			return "Sammy Corporation\0";
		case 0xC0:
		case 0xD0:
			return "Taito\0";
		case 0xC2:
			return "Kemco\0";
		case 0xC3:
			return "Square\0";
		case 0xC4:
			return "Tokuma Shoten\0";
		case 0xC5:
			return "Data East\0";
		case 0xC6:
			return "Tonkin House\0";
		case 0xC8:
			return "Koei\0";
		case 0xC9:
			return "UFL\0";
		case 0xCA:
			return "Ultra Games\0";
		case 0xCB:
			return "VAP, Inc.\0";
		case 0xCC:
			return "Use Corporation\0";
		case 0xCD:
			return "Meldac\0";
		case 0xD1:
			return "SOFEL (Software Engineering Lab)\0";
		case 0xD2:
			return "Quest\0";
		case 0xD3:
			return "Sigma Enterprises\0";
		case 0xD4:
			return "ASK Kodansha Co.\0";
		case 0xD6:
			return "Naxat Soft\0";
		case 0xD7:
			return "Copya System\0";
		case 0xDA:
			return "Tomy\0";
		case 0xDD:
			return "Nippon Computer Systems\0";
		case 0xDE:
			return "Human Ent.\0";
		case 0xDF:
			return "Altron\0";
		case 0xE1:
			return "Towa Chiki\0";
		case 0xE2:
			return "Yukata\0";
		case 0xE3:
			return "Varie\0";
		case 0xE5:
			return "Epoch\0";
		case 0xE7:
			return "Athena\0";
		case 0xE8:
			return "Asmik Ace Entertainment\0";
		case 0xE9:
			return "Natsume\0";
		case 0xEA:
			return "King Records\0";
		case 0xEC:
			return "Epic/Sony Records\0";
		case 0xEE:
			return "IGS\0";
		case 0xF0:
			return "A Wave\0";
		case 0xF3:
			return "Extreme Entertainment\0";
		default:
			return UNMATCHED_ERROR;
	}
}

char * getNewLicenseeCode(uint8_t oldLicenseeCode, uint16_t newLicenseeCode)
{
	if (oldLicenseeCode == 0x33) {
		return "-\0";
	}

	switch (newLicenseeCode)
	{
		case 0x3030:
			return "None\0";
		case 0x3031:
			return "Nintendo Research & Development 1\0";
		case 0x3038:
			return "Capcom\0";
		case 0x3133:
		case 0x3639:
			return "EA (Electronic Arts)\0";
		case 0x3138:
			return "Hudson Soft\0";
		case 0x3139:
			return "B-AI\0";
		case 0x3230:
			return "KSS\0";
		case 0x3232:
			return "Planning Office WADA\0";
		case 0x3234:
			return "PCM Complete\0";
		case 0x3235:
			return "San-X\0";
		case 0x3238:
			return "Kemco\0";
		case 0x3239:
			return "SETA Corporation\0";
		case 0x3330:
			return "Viacom\0";
		case 0x3331:
			return "Nintendo\0";
		case 0x3332:
			return "Bandai\0";
		case 0x3333:
		case 0x3933:
			return "OceanSoftware/Acclaim Entertainment\0";
		case 0x3334:
		case 0x3534:
			return "Konami\0";
		case 0x3335:
			return "Hector Soft\0";
		case 0x3337:
			return "Taito\0";
		case 0x3338:
			return "Hudson Soft\0";
		case 0x3339:
			return "Banpresto\0";
		case 0x3431:
			return "Ubi Soft\0";
		case 0x3432:
			return "Atlus\0";
		case 0x3434:
			return "Malibu Interactive\0";
		case 0x3436:
			return "Angel\0";
		case 0x3437:
			return "Bullet-Proof Software\0";
		case 0x3439:
			return "Irem\0";
		case 0x3530:
			return "Absolute\0";
		case 0x3531:
			return "Acclaim Entertainment\0";
		case 0x3532:
			return "Activision\0";
		case 0x3533:
			return "Sammy USA Corporation\0";
		case 0x3535:
			return "Hi Tech Expressions\0";
		case 0x3536:
			return "LJN\0";
		case 0x3537:
			return "MatchBox\0";
		case 0x3538:
			return "Mattel\0";
		case 0x3539:
			return "Milton Bradley Company\0";
		case 0x3630:
			return "Titus Interactive\0";
		case 0x3631:
			return "Virgin Games Ltd.\0";
		case 0x3634:
			return "Lucasfilm Games\0";
		case 0x3637:
			return "Ocean Software\0";
		case 0x3730:
			return "Infogrames\0";
		case 0x3731:
			return "Interplay Entertainment\0";
		case 0x3732:
			return "Broderbund\0";
		case 0x3733:
			return "Sculptured Software\0";
		case 0x3735:
			return "The Sales Curve Limited\0";
		case 0x3738:
			return "THQ\0";
		case 0x3739:
			return "Accolade\0";
		case 0x3830:
			return "Misawa entertainment\0";
		case 0x3833:
			return "LOZC G.\0";
		case 0x3836:
			return "Tokuma Shoten\0";
		case 0x3837:
			return "Tsukuda Original\0";
		case 0x3931:
			return "Chunsoft Co.\0";
		case 0x3932:
			return "Video System\0";
		case 0x3935:
			return "Varie\0";
		case 0x3936:
			return "Yonezawa\0";
		case 0x3937:
			return "Kaneko\0";
		case 0x3939:
			return "Pack-In-Video\0";
		case 0x3948:
			return "Bottom Up\0";
		case 0x3134:
			return "Konami (Yu-Gi-Oh!)\0";
		case 0x424C:
			return "MTO\0";
		case 0x444B:
			return "Kodansha\0";
		default:
			return UNMATCHED_ERROR;
	}
}

char * sgbFlagToString(uint8_t sgbFlag)
{
	return sgbFlag == SGB_FLAG ? "yes\0" : "no\0";
}

void printCartridgeInformation(struct Cartridge *cartridge)
{
	if (cartridge->logFile == NULL) {
		fprintf(stderr, "[CARTRIDGE] Error printing cartridge metadata: logFile is NULL\n");	
		return;
	}
	fprintf(cartridge->logFile, "[CARTRIDGE] Cartridge metadata:\n");
	fprintf(cartridge->logFile, "Title: %s\n", cartridge->title);
	fprintf(cartridge->logFile, "Manufacturer code: %c%c%c%c\n", 
			cartridge->manufacturerCode[0x0],
			cartridge->manufacturerCode[0x1],
			cartridge->manufacturerCode[0x2],
			cartridge->manufacturerCode[0x3]);
	fprintf(cartridge->logFile, "CGB flag: %s\n", cgbFlagToString(cartridge->cgbFlag));
	fprintf(cartridge->logFile, "New Licensee Code: %s\n", 
			getNewLicenseeCode(cartridge->oldLicenseeCode, cartridge->newLicenseeCode));
	fprintf(cartridge->logFile, "SGB supported: %s\n", sgbFlagToString(cartridge->sgbFlag));
	fprintf(cartridge->logFile, "Cartridge type: %s\n", cartridgeTypeToString(cartridge->cartridgeType));
	fprintf(cartridge->logFile, "ROM size: %s\n", romSizeToString(cartridge->romSize));
	fprintf(cartridge->logFile, "RAM size: %s\n", ramSizeToString(cartridge->ramSize));
	fprintf(cartridge->logFile, "Destination code: %s\n", destinationCodeToString(cartridge->destinationCode));
	fprintf(cartridge->logFile, "Old Licensee Code: %s\n", getOldLicenseeCode(cartridge->oldLicenseeCode));
	fprintf(cartridge->logFile, "ROM version: %d\n", cartridge->romVersion);
	fprintf(cartridge->logFile, "Header checksum value: 0x%02hhX\n", cartridge->headerChecksum);
	fprintf(cartridge->logFile, "Global checksum value: 0x%02hhX\n", cartridge->globalChecksum);
}

