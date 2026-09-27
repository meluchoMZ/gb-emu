/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../../src/soc/cpu.h"
#include "../../../src/soc/cpu_instructions.h"
#include "../../../src/soc/memory.h"

#include "utils.h"

#include <ctest_api.h>

TEST(LoadOperations, load0x06, "LD B, n - loads the data n in B register")
{
	INIT_ENV;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.PC);
	cpu.B = 0x00;
	mmap.bootRomEnabled = false;
	uint16_t oldPC = cpu.PC;
	uint16_t oldB = cpu.B;
	writeMemory(&mmap, cpu.PC + 1, 0x6E);
	EXEC(0x0, 0x6)
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x02, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x6E, cpu.B);
	cpu.PC = oldPC;
	cpu.B = oldB;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x08, "LD (nn), SP - Write the value of SP to the address nn")
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

TEST(LoadOperations, load0x0E, "LD C, n - loads the data n in the C register")
{
	INIT_ENV;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x00, cpu.PC);
	cpu.C = 0x00;
	uint16_t oldPC = cpu.PC;
	uint16_t oldC = cpu.C;
	mmap.bootRomEnabled = false;
	writeMemory(&mmap, cpu.PC + 1, 0x6E);
	EXEC(0x0, 0xE)
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x02, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x6E, cpu.C);
	cpu.PC = oldPC;
	cpu.C = oldC;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x11, "LD DE, nn - loads the data nn into the register DE")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.DE = 0x0000;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x0000, cpu.DE);
	uint16_t oldPC = cpu.PC;
	uint16_t oldDE = cpu.DE;
	writeMemory(&mmap, cpu.PC + 1, 0xED);
	writeMemory(&mmap, cpu.PC + 2, 0xDE);
	EXEC(0x1, 0x1)
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(cpu.PC, 0x1003);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(cpu.DE, 0xDEED);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(cpu.D, 0xDE);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(cpu.E, 0xED);
	cpu.PC = oldPC;
	cpu.DE = oldDE;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x1A, "LD A, (DE) - loads into the register A the data pointed by the DE register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.DE = 0x2000;
	writeMemory(&mmap, cpu.DE, 0x1A);
	uint8_t oldA = cpu.A;
	uint16_t oldPC = cpu.PC;
	EXEC(0x1, 0xA);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(cpu.PC, 0x1001);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(cpu.A, 0x1A);
	cpu.PC = oldPC;
	cpu.A = oldA;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x21, "LD HL, nn - loads to the register HL the immediate data nn")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.HL = 0x0000;
	// write lsb
	writeMemory(&mmap, cpu.PC + 1, 0x21);
	// write msb
	writeMemory(&mmap, cpu.PC + 2, 0x12);
	uint16_t oldPC = cpu.PC;
	uint16_t oldHL = cpu.HL;
	EXEC(0x2, 0x1);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1003, cpu.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1221, cpu.HL);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x21, cpu.L);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x12, cpu.H);
	cpu.PC = oldPC;
	cpu.HL = oldHL;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x26, "LD H, n - loads the data n to the register H")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.H = 0x00;
	writeMemory(&mmap, cpu.PC + 1, 0x2F);
	uint16_t oldPC = cpu.PC;
	uint8_t oldH = cpu.H;
	EXEC(0x2, 0x6);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1002, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x2F, cpu.H);
	cpu.PC = oldPC;
	cpu.H = oldH;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x31, "LD SP, nn - loads to the Stack Pointer the immediate data nn")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.SP = 0x0000;
	// write lsb
	writeMemory(&mmap, cpu.PC + 1, 0x31);
	// write msb
	writeMemory(&mmap, cpu.PC + 2, 0x13);
	uint16_t oldPC = cpu.PC;
	uint16_t oldSP = cpu.SP;
	EXEC(0x3, 0x1);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1003, cpu.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1331, cpu.SP);
	cpu.PC = oldPC;
	cpu.SP = oldSP;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x32, "LD (HL-), A - loads the data in A to the address specified by HL. Then HL is decremented")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0xAA;
	cpu.HL = 0x3333;
	uint16_t oldPC = cpu.PC;
	uint16_t oldHL = cpu.HL;
	uint8_t oldA = cpu.A;
	writeMemory(&mmap, 0x3333, 0x00);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0x00, readMemory(&mmap, 0x3333));
	EXEC(0x3, 0x2);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x3332, cpu.HL);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, cpu.A);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, readMemory(&mmap, 0x3333));
	cpu.A = oldA;
	cpu.PC = oldPC;
	cpu.HL = oldHL;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x3E, "LD A, n - loads the data n into the A register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0x00;
	writeMemory(&mmap, cpu.PC + 1, 0xAA);
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x3, 0xE);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1002, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, cpu.A);
	cpu.PC = oldPC;
	cpu.A = oldA;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x4F, "LD C, A - loads the data from A register into C register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0xAA;
	cpu.C = 0x00;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	uint8_t oldC = cpu.C;
	EXEC(0x4, 0xF);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, cpu.A);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, cpu.C);
	cpu.PC = oldPC;
	cpu.A = oldA;
	cpu.C = oldC;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x63, "LD H, E - loads the data from H register into E register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.E = 0xEE;
	cpu.H = 0x00;
	uint16_t oldPC = cpu.PC;
	uint8_t oldE = cpu.E;
	uint8_t oldH = cpu.H;
	EXEC(0x6, 0x3);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xEE, cpu.E);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xEE, cpu.H);
	cpu.PC = oldPC;
	cpu.E = oldE;
	cpu.H = oldH;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x66, "LD H, (HL) - loads the data from the address in HL to the H register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.HL = 0x3000;
	writeMemory(&mmap, 0x3000, 0xF1);
	uint16_t oldPC = cpu.PC;
	uint8_t oldH = cpu.H;
	EXEC(0x6, 0x6);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xF1, cpu.H);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0xF100, cpu.HL);
	cpu.PC = oldPC;
	cpu.H = oldH;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x67, "LD H, A - loads the data from H register into A register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0xAA;
	cpu.H = 0x00;
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	uint8_t oldH = cpu.H;
	EXEC(0x6, 0x7);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, cpu.A);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, cpu.H);
	cpu.PC = oldPC;
	cpu.A = oldA;
	cpu.H = oldH;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x6E, "LD L, (HL) - loads the data from the address in HL to the L register")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.HL = 0x2033;
	writeMemory(&mmap, 0x2033, 0xF1);
	uint16_t oldPC = cpu.PC;
	uint8_t oldL = cpu.L;
	EXEC(0x6, 0xE);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xF1, cpu.L);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x20F1, cpu.HL);
	cpu.PC = oldPC;
	cpu.L = oldL;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x73, "LD, (HL), E - loads the data in E into the address pointed by HL")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.HL = 0x2000;
	cpu.E = 0xEE;
	writeMemory(&mmap, 0x2100, 0x00);
	uint16_t oldPC = cpu.PC;
	uint8_t oldE = cpu.E;
	EXEC(0x7, 0x3);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xEE, cpu.E);
	cpu.PC = oldPC;
	cpu.E = oldE;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0x77, "LD, (HL), A - loads the data in A into the address pointed by HL")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.HL = 0x2000;
	cpu.A = 0xAA;
	writeMemory(&mmap, 0x2100, 0x00);
	uint16_t oldPC = cpu.PC;
	uint8_t oldA = cpu.A;
	EXEC(0x7, 0x3);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, cpu.A);
	cpu.PC = oldPC;
	cpu.A = oldA;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0xE0, "LDH (n), A - loads from accumulatro to 0xFF00 + n offset")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0xAA;
	writeMemory(&mmap, 0x1001, 0x33);
	uint16_t oldPC = cpu.PC;
	EXEC(0xE, 0x0);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1002, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, readMemory(&mmap, 0xFF33));
	cpu.PC = oldPC;
	assertCPUEquals(expected, cpu);
}

TEST(LoadOperations, load0xE2, "LDH (C), A - loads from accumulatro to 0xFF00 + C offset")
{
	INIT_ENV;
	cpu.PC = 0x1000;
	cpu.A = 0xAA;
	cpu.C = 0x33;
	uint16_t oldPC = cpu.PC;
	EXEC(0xE, 0x2);
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(0x1001, cpu.PC);
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(0xAA, readMemory(&mmap, 0xFF33));
	cpu.PC = oldPC;
	assertCPUEquals(expected, cpu);
}
