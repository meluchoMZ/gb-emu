/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../../src/soc/cpu.h"
#include "../../../src/soc/cpu_instructions.h"
#include "../../../src/soc/memory.h"

#include <ctest_api.h>
#include <stdbool.h>

bool initEnvironment(struct CPU *cpu, struct MMAP *mmap, FILE *logFile)
{
	ASSERT_TRUE(initCPU(cpu, logFile));
	ASSERT_TRUE(initMemoryMap(mmap, logFile));
	return true;
}

bool cloneCPU(struct CPU *original, struct CPU *cloned)
{
	ASSERT_NON_NULL(original);
	ASSERT_NON_NULL(cloned);
	cloned->logFile = original->logFile;
	cloned->PC = original->PC;
	cloned->SP = original->SP;
	cloned->AF = original->AF;
	cloned->BC = original->BC;
	cloned->DE = original->DE;
	cloned->HL = original->HL;
	cloned->IME = original->IME;
	cloned->cbPrefix = original->cbPrefix;
	return true;
}

void assertCPUEquals(struct CPU expected, struct CPU actual)
{
	ASSERT_POINTER_EQUALS(expected.logFile, actual.logFile);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected.PC, actual.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected.SP, actual.SP);
	// comparison from smaller to bigger unions
	ASSERT_TRUE(expected.F.C == actual.F.C);
	ASSERT_TRUE(expected.F.H == actual.F.H);
	ASSERT_TRUE(expected.F.N == actual.F.N);
	ASSERT_TRUE(expected.F.Z == actual.F.Z);

	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected.A, actual.A);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected.C, actual.C);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected.B, actual.B);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected.E, actual.E);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected.D, actual.D);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected.L, actual.L);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected.H, actual.H);

	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected.AF, actual.AF);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected.BC, actual.BC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected.DE, actual.DE);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected.HL, actual.HL);

	ASSERT_TRUE(expected.IME == actual.IME);
	ASSERT_TRUE(expected.cbPrefix == actual.cbPrefix);
}
