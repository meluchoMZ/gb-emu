/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../../src/soc/cpu.h"
#include "../../../src/soc/cpu_instructions.h"
#include "../../../src/soc/memory.h"

#include "utils.h"

#include <ctest_api.h>

TEST(BitwiseOperations, op0x17, "RLA - rotates the acumulator left through the carry flag")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0b00111111;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x1, 0x7);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0b01111110, cpu.A);
	ASSERT_TRUE(0 == cpu.F.Z);
	ASSERT_TRUE(0 == cpu.F.N);
	ASSERT_TRUE(0 == cpu.F.H);
	ASSERT_TRUE(0 == cpu.F.C);
	cpu.PC = oldPC;
	cpu.A = oldA;
	assertCPUEquals(expected, cpu);
}

TEST(BitwiseOperations, op0x17throughCarry, "RLA - rotates the acumulator left through the carry flag with previous carry value")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0b00111111;
	cpu.F.C = 0b1;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x1, 0x7);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0b01111111, cpu.A);
	ASSERT_TRUE(0 == cpu.F.Z);
	ASSERT_TRUE(0 == cpu.F.N);
	ASSERT_TRUE(0 == cpu.F.H);
	ASSERT_TRUE(0 == cpu.F.C);
	cpu.PC = oldPC;
	cpu.A = oldA;
	cpu.F.C = 0b1;
	assertCPUEquals(expected, cpu);
}

TEST(BitwiseOperations, op0x17_Carries, "RLA - rotates the acumulator left through the carry flag generates carry")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0xFF;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x1, 0x7);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0b11111110, cpu.A);
	ASSERT_TRUE(0 == cpu.F.Z);
	ASSERT_TRUE(0 == cpu.F.N);
	ASSERT_TRUE(0 == cpu.F.H);
	ASSERT_TRUE(1 == cpu.F.C);
	cpu.PC = oldPC;
	cpu.A = oldA;
	cpu.F.C = 0b0;
	assertCPUEquals(expected, cpu);
}

TEST(BitwiseOperations, op0x1F, "RRA - rotates the acumulator right through the carry flag")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0b00111110;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x1, 0xF);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0b00011111, cpu.A);
	ASSERT_TRUE(0 == cpu.F.Z);
	ASSERT_TRUE(0 == cpu.F.N);
	ASSERT_TRUE(0 == cpu.F.H);
	ASSERT_TRUE(0 == cpu.F.C);
	cpu.PC = oldPC;
	cpu.A = oldA;
	assertCPUEquals(expected, cpu);
}

TEST(BitwiseOperations, op0x1FthroughCarry, "RRA - rotates the acumulator right through the carry flag with previous carry value")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0b00111100;
	cpu.F.C = 0b1;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x1, 0xF);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0b10011110, cpu.A);
	ASSERT_TRUE(0 == cpu.F.Z);
	ASSERT_TRUE(0 == cpu.F.N);
	ASSERT_TRUE(0 == cpu.F.H);
	ASSERT_TRUE(0 == cpu.F.C);
	cpu.PC = oldPC;
	cpu.A = oldA;
	cpu.F.C = 0b1;
	assertCPUEquals(expected, cpu);
}

TEST(BitwiseOperations, op0x1F_Carries, "RRA - rotates the acumulator right through the carry flag generates carry")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0xFF;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x1, 0xF);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0b01111111, cpu.A);
	ASSERT_TRUE(0 == cpu.F.Z);
	ASSERT_TRUE(0 == cpu.F.N);
	ASSERT_TRUE(0 == cpu.F.H);
	ASSERT_TRUE(1 == cpu.F.C);
	cpu.PC = oldPC;
	cpu.A = oldA;
	cpu.F.C = 0b0;
	assertCPUEquals(expected, cpu);
}
