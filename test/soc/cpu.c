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

TEST(CPU, memoryOverlapAFRegisters, "Test that memory is overlapped in the struct CPU definition so that any write to A, F, or AF register writes to the same memory")
{
	struct CPU cpu;
	FILE *logFile = stdout;
	ASSERT_TRUE(initCPU(&cpu, logFile));
	ASSERT_POINTER_EQUALS((void *) (&cpu.F + 1), (void *) &cpu.A);
	ASSERT_POINTER_EQUALS((void *) &cpu.AF, (void *) &(cpu.F));
	cpu.AF = 0x0000;
	ASSERT_TRUE(0x0 == cpu.F._unused);
	ASSERT_TRUE(0x0 == cpu.F.C);
	ASSERT_TRUE(0x0 == cpu.F.H);
	ASSERT_TRUE(0x0 == cpu.F.N);
	ASSERT_TRUE(0x0 == cpu.F.Z);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.A);
	cpu.A = 0xAF;
	cpu.F.C = 0x1;
	cpu.F.H = 0x1;
	cpu.F.N = 0x1;
	cpu.F.Z = 0x1;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAF, cpu.A);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xAFF0, cpu.AF);
}

TEST(CPU, memoryOverlapBCRegisters, "Test that memory is overlapped in the struct CPU definition so that any write to B, C, or BC register writes to the same memory")
{
	struct CPU cpu;
	FILE *logFile = stdout;
	ASSERT_TRUE(initCPU(&cpu, logFile));
	ASSERT_POINTER_EQUALS((void *) (&cpu.C + 1), (void *) &cpu.B);
	ASSERT_POINTER_EQUALS((void *) &cpu.BC, (void *) &cpu.C);
	cpu.BC = 0x0000;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.C);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.B);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.BC);
	cpu.B = 0xBF;
	cpu.C = 0x1C;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x1C, cpu.C);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xBF, cpu.B);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xBF1C, cpu.BC);
}

TEST(CPU, memoryOverlapDERegisters, "Test that memory is overlapped in the struct CPU definition so that any write to D, E, or DE register writes to the same memory")
{
	struct CPU cpu;
	FILE *logFile = stdout;
	ASSERT_TRUE(initCPU(&cpu, logFile));
	ASSERT_POINTER_EQUALS((void *) (&cpu.E + 1), (void *) &cpu.D);
	ASSERT_POINTER_EQUALS((void *) &cpu.DE, (void *) &cpu.E);
	cpu.HL = 0x0000;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.E);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.D);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.DE);
	cpu.D = 0xBF;
	cpu.E = 0x1C;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x1C, cpu.E);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xBF, cpu.D);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xBF1C, cpu.DE);
}

TEST(CPU, memoryOverlapHLRegisters, "Test that memory is overlapped in the struct CPU definition so that any write to H, L, or HL register writes to the same memory")
{
	struct CPU cpu;
	FILE *logFile = stdout;
	ASSERT_TRUE(initCPU(&cpu, logFile));
	ASSERT_POINTER_EQUALS((void *) (&cpu.L + 1), (void *) &cpu.H);
	ASSERT_POINTER_EQUALS((void *) &cpu.HL, (void *) &cpu.L);
	cpu.HL = 0x0000;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.L);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.H);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.HL);
	cpu.H = 0xBF;
	cpu.L = 0x1C;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x1C, cpu.L);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xBF, cpu.H);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xBF1C, cpu.HL);
}
