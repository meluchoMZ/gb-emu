/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../../src/soc/cpu.h"
#include "../../../src/soc/cpu_instructions.h"
#include "../../../src/soc/memory.h"

#include "utils.h"

#include <ctest_api.h>

TEST(Loads, load0x06, "LD B, n - loads the data n in B register")
{
	INIT_ENV;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.PC);
	cpu.B = 0x00;
	mmap.bootRomEnabled = false;
	writeMemory(&mmap, cpu.PC + 1, 0x6E);
	EXEC(0x0, 0x6)
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x02, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x6E, cpu.B);
}

TEST(Loads, load0x08, "LD (nn), SP- Write the value of SP to the address nn")
{
	// Test OP 0x082120, PC = 1000, SP = 0xFB1B
	// nn = 2021
	// op will write 0x1B to memory at 0x2021
	// op will write 0xFB to memory at 0x2022
	// PC will increment by 3 (3 byte opcode)
	// SP will stay the same
	// all other registers should be unchanged
	INIT_ENV;
	// set initial PC and SP
	cpu.PC = 0x1000;
	cpu.SP = 0xFB1B;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1000, cpu.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xFB1B, cpu.SP);

	uint16_t oldPC = cpu.PC;
	uint16_t oldSP = cpu.SP;
	// reset target memory
	writeMemory(&mmap, 0x2021, 0x00);
	writeMemory(&mmap, 0x2022, 0x00);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, readMemory(&mmap, 0x2021));
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, readMemory(&mmap, 0x2022));
	// set memory to read from pc increments
	writeMemory(&mmap, 0x1001, 0x21);
	writeMemory(&mmap, 0x1002, 0x20);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x21, readMemory(&mmap, 0x1001));
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x20, readMemory(&mmap, 0x1002));
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1000, cpu.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xFB1B, cpu.SP);
	// exec load
	EXEC(0x0, 0x8);
	// check PC, SP and memory values
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1003, cpu.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xFB1B, cpu.SP);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x1B, readMemory(&mmap, 0x2021));
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xFB, readMemory(&mmap, 0x2022));
	// reset PC, SP to before test values
	cpu.PC = oldPC;
	cpu.SP = oldSP;
	// check that no other value changed in the cpu
	assertCPUEquals(expected, cpu);
}

