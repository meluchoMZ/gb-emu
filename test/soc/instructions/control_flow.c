/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../../src/soc/cpu.h"
#include "../../../src/soc/cpu_instructions.h"
#include "../../../src/soc/memory.h"

#include "utils.h"

#include <ctest_api.h>

TEST(ControlFlowOperations, op0x20_NoJump, "JR NZ, e - conditional relative jump to e if NZ is met - no jump")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.F.Z = 0b1;
	writeMemory(&mmap, 0x1001, 0x1D);
	uint16_t oldPC = cpu.PC;
	EXEC(0x2, 0x0);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1002, cpu.PC);
	cpu.PC = oldPC;
	assertCPUEquals(expected, cpu);
}

TEST(ControlFlowOperations, op0x20_Jumps, "JR NZ, e - conditional relative jump to e if NZ is met - jumps")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.F.Z = 0b0;
	writeMemory(&mmap, 0x1001, 0x1D);
	uint16_t oldPC = cpu.PC;
	EXEC(0x2, 0x0);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x101F, cpu.PC);
	cpu.PC = oldPC;
	assertCPUEquals(expected, cpu);
}

TEST(ControlFlowOperations, op0x20_JumpsBackwards, "JR NZ, e - conditional relative jump to e if NZ is met - jumps backwards")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.F.Z = 0b0;
	writeMemory(&mmap, 0x1001, 0xF6); // -10 
	uint16_t oldPC = cpu.PC;
	EXEC(0x2, 0x0);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x0FF8, cpu.PC);
	cpu.PC = oldPC;
	assertCPUEquals(expected, cpu);
}
