/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../../src/soc/cpu.h"
#include "../../../src/soc/cpu_instructions.h"
#include "../../../src/soc/memory.h"

#include "utils.h"

#include <ctest_api.h>

TEST(ArithmeticAndLogicalOperations, op0x03, "INC BC - Increments BC register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.BC = 0xBBBB;
	uint16_t oldPC = cpu.PC;
	uint16_t oldBC = cpu.BC;
	EXEC(0x0, 0x3);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xBBBC, cpu.BC);
	cpu.PC = oldPC;
	cpu.BC = oldBC;
	assertCPUEquals(expected, cpu);
}

TEST(ArithmeticAndLogicalOperations, op0x0B, "DEC BC - Decrements BC register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.BC = 0xBBBC;
	uint16_t oldPC = cpu.PC;
	uint16_t oldBC = cpu.BC;
	EXEC(0x0, 0xB);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xBBBB, cpu.BC);
	cpu.PC = oldPC;
	cpu.BC = oldBC;
	assertCPUEquals(expected, cpu);
}

TEST(ArithmeticAndLogicalOperations, op0x0C, "INC C - Increments C register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.C = 0xBB;
	uint16_t oldPC = cpu.PC;
	uint16_t oldC = cpu.C;
	EXEC(0x0, 0xC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xBC, cpu.C);
	ASSERT_TRUE(cpu.F.N == 0x0);
	ASSERT_TRUE(cpu.F.H == 0x0);
	ASSERT_TRUE(cpu.F.C == 0x0);
	ASSERT_TRUE(cpu.F.Z == 0x0);
	cpu.PC = oldPC;
	cpu.C = oldC;
	assertCPUEquals(expected, cpu);
}

TEST(ArithmeticAndLogicalOperations, op0x0C_Carries, "INC C - Increments C register testing the carrying logic. Also tests zero flag activation")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.C = 0xFF;
	uint16_t oldPC = cpu.PC;
	uint16_t oldC = cpu.C;
	EXEC(0x0, 0xC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.C);
	ASSERT_TRUE(cpu.F.N == 0x0);
	ASSERT_TRUE(cpu.F.H == 0x1);
	ASSERT_TRUE(cpu.F.C == 0x1);
	ASSERT_TRUE(cpu.F.Z == 0x1);
	cpu.PC = oldPC;
	cpu.C = oldC;
	cpu.F.H = 0x0;
	cpu.F.C = 0x0;
	cpu.F.Z = 0x0;
	assertCPUEquals(expected, cpu);
}

TEST(ArithmeticAndLogicalOperations, op0x0D, "DEC C - Decrements C register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.C = 0xBC;
	uint16_t oldPC = cpu.PC;
	uint16_t oldC = cpu.C;
	EXEC(0x0, 0xD);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xBB, cpu.C);
	ASSERT_TRUE(cpu.F.N == 0x1);
	ASSERT_TRUE(cpu.F.H == 0x0);
	ASSERT_TRUE(cpu.F.C == 0x0);
	ASSERT_TRUE(cpu.F.Z == 0x0);
	cpu.PC = oldPC;
	cpu.C = oldC;
	cpu.F.N = 0x0;
	assertCPUEquals(expected, cpu);
}

TEST(ArithmeticAndLogicalOperations, op0x0D_Borrows, "DEC C - Decrements C register testing the bit borrowing logic")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.C = 0x00;
	uint16_t oldPC = cpu.PC;
	uint16_t oldC = cpu.C;
	EXEC(0x0, 0xD);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xFF, cpu.C);
	ASSERT_TRUE(cpu.F.N == 0x1);
	ASSERT_TRUE(cpu.F.H == 0x1);
	ASSERT_TRUE(cpu.F.C == 0x1);
	ASSERT_TRUE(cpu.F.Z == 0x0);
	cpu.PC = oldPC;
	cpu.C = oldC;
	cpu.F.N = 0x0;
	cpu.F.H = 0x0;
	cpu.F.C = 0x0;
	assertCPUEquals(expected, cpu);
}

TEST(ArithmeticAndLogicalOperations, op0x0D_Zero, "DEC C - Decrements C register testing the zero flag activation")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.C = 0x01;
	uint16_t oldPC = cpu.PC;
	uint16_t oldC = cpu.C;
	EXEC(0x0, 0xD);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.C);
	ASSERT_TRUE(cpu.F.N == 0x1);
	ASSERT_TRUE(cpu.F.H == 0x0);
	ASSERT_TRUE(cpu.F.C == 0x0);
	ASSERT_TRUE(cpu.F.Z == 0x1);
	cpu.PC = oldPC;
	cpu.C = oldC;
	cpu.F.N = 0x0;
	cpu.F.Z = 0x0;
	assertCPUEquals(expected, cpu);
}

TEST(ArithmeticAndLogicalOperations, op0x33, "INC SP - Increments the stack pointer value")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.SP = 0x3333;
	uint16_t oldPC = cpu.PC;
	uint16_t oldSP = cpu.SP;
	EXEC(0x3, 0x3);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x3334, cpu.SP);
	cpu.PC = oldPC;
	cpu.SP = oldSP;
	assertCPUEquals(expected, cpu);
}

TEST(ArithmeticAndLogicalOperations, op0x83, "ADD E - Adds the value of the register E to the accumulator (A register)")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.E = 0xE3;
	cpu.A = 0x02;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x8, 0x3);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xE5, cpu.A);
	ASSERT_TRUE(cpu.F.C == 0b0);
	ASSERT_TRUE(cpu.F.H == 0b0);
	ASSERT_TRUE(cpu.F.N == 0b0);
	ASSERT_TRUE(cpu.F.Z == 0b0);
	cpu.PC = oldPC;
	cpu.A = oldA;
	assertCPUEquals(expected, cpu);
}

TEST(ArithmeticAndLogicalOperations, op0x83_CarriesAndZero, "ADD E - Adds the value of the register E to the accumulator (A register). Carries and sums zero")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.E = 0xFF;
	cpu.A = 0x01;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x8, 0x3);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.A);
	ASSERT_TRUE(cpu.F.C == 0b1);
	ASSERT_TRUE(cpu.F.H == 0b1);
	ASSERT_TRUE(cpu.F.N == 0b0);
	ASSERT_TRUE(cpu.F.Z == 0b1);
	cpu.PC = oldPC;
	cpu.A = oldA;
	cpu.F.C = 0b0;
	cpu.F.H = 0b0;
	cpu.F.N = 0b0;
	cpu.F.Z = 0b0;
	assertCPUEquals(expected, cpu);
}
