/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../src/soc/memory.h"

#include <ctest_api.h>

TEST(Memory, initMemoryMap, "Test memory map initialization and initial status")
{
	struct MMAP mmap;
	FILE *logFile = stdout;
	ASSERT_TRUE(initMemoryMap(&mmap, logFile));
	// check memory locations mappings
	uint8_t *memoryBufferBaseAddress = mmap.memoryBuffer;
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + IER), (void *) mmap.interruptEnableRegister);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + HIRAM), (void *) mmap.highInternalRAM);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + EUIO2), (void *) mmap.emptyUnusableForIO2);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + IOPORT), (void *) mmap.IOPorts);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + EUIO), (void *) mmap.emptyUnusableForIO);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + SAM), (void *) mmap.spriteAttribMemory);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + ERAM), (void *) mmap.echoRAM);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + RAM), (void *) mmap.internalRAM);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + SRAM), (void *) mmap.switchableRAMBank);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + VRAM), (void *) mmap.videoRAM);
	ASSERT_POINTER_EQUALS((void *) (memoryBufferBaseAddress + SROM), (void *) mmap.switchableROMBank);
	ASSERT_POINTER_EQUALS((void *) memoryBufferBaseAddress, (void *) mmap.ROMBank);

	// check initial values
	ASSERT_TRUE(mmap.bootRomEnabled);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, mmap.memoryBuffer[R_BOOT_ROM_MAPPING]);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x0F, mmap.memoryBuffer[R_JOYPAD_INPUT]);
}

TEST_EXPECT_FAIL(Memory, initMemoryMapNullMMAP, "Tests memory map initialization passing a null mmap pointer", "[MEMORY] Error initializing memory: memory map is NULL")
{
	FILE *logFile = stdout;
	ASSERT_FALSE(initMemoryMap(NULL, logFile));
}

TEST_EXPECT_FAIL(Memory, initMemoryMapNullLogFile, "Tests memory map initialization passing a null logFile pointer", "[MEMORY] Error initializing memory: log file is NULL")
{
	struct MMAP mmap;
	ASSERT_FALSE(initMemoryMap(&mmap, NULL));
}

TEST(Memory, readMemory, "Tests that a random address in the memory (not boot rom mapping address space) is correctly read")
{
	struct MMAP mmap;
	FILE *logFile = stdout;
	uint8_t data = 0x35;
	uint16_t address = 0x239F;
	ASSERT_TRUE(initMemoryMap(&mmap, logFile));
	mmap.memoryBuffer[address] = data;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(data, readMemory(&mmap, address));
}

TEST(Memory, readBootRomAddressBootRomEnabled, "Tests that a address in the boot rom is corretly read")
{
	struct MMAP mmap;
	FILE *logFile = stdout;
	uint8_t data = 0x2E;
	uint16_t address = 0x000F;
	ASSERT_TRUE(initMemoryMap(&mmap, logFile));
	mmap.bootRom[address] = data;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(data, readMemory(&mmap, address));
}

TEST(Memory, readBootRomAddressBootRomDisabled, "Tests that a address in the boot rom space is corretly read when the boot rom is unmapped")
{
	struct MMAP mmap;
	FILE *logFile = stdout;
	uint8_t dataBootRom = 0x2E;
	uint8_t dataRam = 0x3A;
	uint16_t address = 0x000F;
	ASSERT_TRUE(initMemoryMap(&mmap, logFile));
	mmap.bootRom[address] = dataBootRom;
	mmap.memoryBuffer[address] = dataRam;
	mmap.bootRomEnabled = false;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(dataRam, readMemory(&mmap, address));
}

TEST(Memory, readJoypadInputInitialState, "Tests that the initial state for the joypad represents 'not pressed'")
{
	struct MMAP mmap;
	FILE *logFile = stdout;
	ASSERT_TRUE(initMemoryMap(&mmap, logFile));
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x0F, readMemory(&mmap, R_JOYPAD_INPUT));
}

TEST(Memory, writeMemory, "Tests that the memory is correctly written in a random direction")
{
	struct MMAP mmap;
	FILE *logFile = stdout;
	uint16_t address = 0x6FA4;
	uint8_t data = 0xA3;
	ASSERT_TRUE(initMemoryMap(&mmap, logFile));
	writeMemory(&mmap, address, data);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(data, mmap.memoryBuffer[address]);
}

TEST(Memory, writeMemoryBootRomBankingRegister, "Tests that writing to the boot rom banking register unmaps the ROM from the RAM ")
{
	struct MMAP mmap;
	FILE *logFile = stdout;
	uint16_t address = R_BOOT_ROM_MAPPING;
	uint8_t data = 0x01;
	ASSERT_TRUE(initMemoryMap(&mmap, logFile));
	ASSERT_TRUE(mmap.bootRomEnabled);
	writeMemory(&mmap, address, data);
	ASSERT_FALSE(mmap.bootRomEnabled);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(data, mmap.memoryBuffer[address]);
}
