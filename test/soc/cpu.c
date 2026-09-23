/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../src/soc/cpu.h"
#include "../../src/soc/cpu_instructions.h"

#include <ctest_api.h>

TEST(CPU, initCPU, "Tests CPU initialization values")
{
	struct CPU cpu;
	FILE *logFile = stdout;
	ASSERT_TRUE(initCPU(&cpu, logFile));
	// check init values
	ASSERT_POINTER_EQUALS((void *) logFile, (void *) cpu.logFile);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x0000, cpu.PC);
	ASSERT_FALSE(cpu.IME);
	ASSERT_FALSE(cpu.cbPrefix);
}

TEST_EXPECT_FAIL(CPU, initCPUNullCPU, "Test CPU initialization fails when CPU is null", "[CPU] Error initializing CPU: cpu is NULL")
{
	FILE *logFile = stdout;
	ASSERT_FALSE(initCPU(NULL, logFile));
}

TEST_EXPECT_FAIL(CPU, initCPUNullLogFile, "Test CPU initialization fails when logFile is NULL", "[CPU] Error initializing CPU: cpu is NULL")
{
	struct CPU cpu;
	ASSERT_FALSE(initCPU(&cpu, NULL));
}
