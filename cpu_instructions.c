/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "cpu.h"
#include "cpu_instructions.h"
#include "memory.h"

#include <stdint.h>

// Tells the compiler to ignore unused parameters
#define IGNORE(x) (void)(x)

/* Carry utils */

bool compute8BitCarry(uint8_t a, uint8_t b)
{
	return ((uint16_t) a + (uint16_t) b) > 0xFF;
}

bool compute8BitHalfCarry(uint8_t a, uint8_t b)
{
	return ((a & 0x0F) + (b & 0x0F)) > 0x0F;
}

bool compute8BitBorrowing(uint8_t a, uint8_t b)
{
	return a < b;
}

bool compute8BitHalfBitBorrowing(uint8_t a, uint8_t b)
{
	return (a & 0x0F) < (b & 0x0F);
}

bool compute16BitCarry(uint16_t a, uint16_t b)
{
	return ((uint32_t) a + (uint32_t) b) > 0xFFFF;
}

bool compute16BitHalfCarry(uint16_t a, uint16_t b)
{
	return ((a & 0x00FF) + (b & 0x00FF)) > 0x00FF;
}

bool compute16BitBorrowing(uint16_t a, uint16_t b)
{
	return a < b;
}

bool compute16BitHalfBitBorrowing(uint16_t a, uint16_t b)
{
	return (a & 0x00FF) < (b & 0x00FF);
}

/* 0x0X */

void op0x00(struct CPU *cpu, struct MMAP *mmap)
{
	// NOP
	// Only advances Program Counter
	IGNORE(mmap);
	cpu->PC++;
}

void op0x03(struct CPU *cpu, struct MMAP *mmap)
{
	// INC BC
	// increments the data on the register BC
	IGNORE(mmap);
	cpu->BC++;
	cpu->PC++;
}

void op0x08(struct CPU *cpu, struct MMAP *mmap)
{
	// LD (nn), SP
	// Loads to the address nn the data from the SP
	cpu->PC++;
	uint8_t newSpLsb = readMemory(mmap, cpu->PC++);
	uint8_t newSpMsb = readMemory(mmap, cpu->PC++);
	uint16_t newSP = (((uint16_t) newSpMsb) << 8) | newSpLsb;
	// write Least Significant Bits
	writeMemory(mmap, newSP++, cpu->SP & 0x00FF);
	// write Most Significant Bits
	writeMemory(mmap, newSP, (cpu->SP & 0xFF00) >> 8);
	cpu->PC++;
}

void op0x0B(struct CPU *cpu, struct MMAP *mmap)
{
	// DEC BC
	// decrements the data on the register BC
	IGNORE(mmap);
	cpu->BC--;
	cpu->PC++;
}

void op0x0C(struct CPU *cpu, struct MMAP *mmap)
{
	// INC C
	// Increment C register
	IGNORE(mmap);
	cpu->F.C = compute8BitCarry(cpu->C, 0x01);
	cpu->F.H = compute8BitHalfCarry(cpu->C, 0x01);
	cpu->C++;
	cpu->F.Z = cpu->C == 0x00;
	cpu->F.N = 0;
	cpu->PC++;
}

void op0x0D(struct CPU *cpu, struct MMAP *mmap)
{
	// DEC C
	// decrements the data on the register C
	IGNORE(mmap);
	cpu->F.C = compute8BitBorrowing(cpu->C, 0x01);	
	cpu->F.H = compute8BitHalfBitBorrowing(cpu->C, 0x01);
	cpu->F.N = 1;
	cpu->C--;
	cpu->PC++;
}

void op0x0E(struct CPU *cpu, struct MMAP *mmap)
{
	// LD C, n
	// Loads the data n to the register C
	cpu->PC++;
	cpu->C = readMemory(mmap, cpu->PC++);
}

/* 0x1X */

void op0x10(struct CPU *cpu, struct MMAP *mmap)
{
	// STOP
	// halts system, so it does not advance Program Counter
	// system will always be processing the same STOP instruction until restart
	IGNORE(cpu);
	IGNORE(mmap);
}

void op0x11(struct CPU *cpu, struct MMAP *mmap)
{
	// LD DE, nn
	// Loads the data nn to the register DE
	cpu->PC++;
	uint8_t dataLsb = readMemory(mmap, cpu->PC++);
	uint8_t dataMsb = readMemory(mmap, cpu->PC++);
	uint16_t data = (((uint16_t) dataMsb) << 8) | dataLsb;
	cpu->DE = data;
}

void op0x1F(struct CPU *cpu, struct MMAP *mmap)
{
	// RRA
	// Rotates A register right through the carry flag
	IGNORE(mmap);
	uint8_t carryValue = cpu->F.C & 0x01;
	cpu->F.C = cpu->A & 0x01;
	cpu->A = cpu->A >> 1;
	cpu->A = cpu->A | (carryValue << 7);
	cpu->F.Z = 0x0;
	cpu->F.N = 0x0;
	cpu->F.H = 0x0;
	cpu->F.C = carryValue;
	cpu->PC++;
}

/* 0x3X*/

void op0x33(struct CPU *cpu, struct MMAP *mmap)
{
	// INC SP
	// Increments the data in the Stack Pointer
	IGNORE(mmap);
	cpu->SP++;
	cpu->PC++;
}

void op0x3E(struct CPU *cpu, struct MMAP *mmap)
{
	// LD A, n
	// Loads the data n to the A register
	cpu->PC++;
	cpu->A = readMemory(mmap, cpu->PC++);
}

/* 0x6X */

void op0x63(struct CPU *cpu, struct MMAP *mmap)
{
	// LD H, E
	// Load to H the data in E
	IGNORE(mmap);
	cpu->H = cpu->E;
	cpu->PC++;
}

void op0x66(struct CPU *cpu, struct MMAP *mmap)
{
	// LD H, (HL)
	// Loads the data pointed by the address value in the HL register and stores it
	// in the H register
	uint8_t data = readMemory(mmap, cpu->HL);
	cpu->H = data;
	cpu->PC++;
}

void op0x67(struct CPU *cpu, struct MMAP *mmap)
{
	// LD H, A
	// Load to H the data in E
	IGNORE(mmap);
	cpu->H = cpu->A;
	cpu->PC++;
}

void op0x6E(struct CPU *cpu, struct MMAP *mmap)
{
	// LD L, (HL)
	// Loads the data pointed by the address value in the HL register and stores it
	// in the L register
	uint8_t data = readMemory(mmap, cpu->HL);
	cpu->L = data;
	cpu->PC++;
}

/* 0x7X */

void op0x73(struct CPU *cpu, struct MMAP *mmap)
{
	// LD (HL), E
	// write the data in E to the address pointed by HL
	writeMemory(mmap, cpu->HL, cpu->E);
	cpu->PC++;
}

/* 0x8X */

void op0x83(struct CPU *cpu, struct MMAP *mmap)
{
	// ADD E
	// Adds the value in the register E to the register A
	IGNORE(mmap);
	cpu->F.C = compute8BitCarry(cpu->A, cpu->E);
	cpu->F.H = compute8BitHalfCarry(cpu->A, cpu->E);
	cpu->A += cpu->E;
	cpu->F.Z = cpu->A == 0x0;
	cpu->F.N = 0x0;
	cpu->PC++;
}

void op0x88(struct CPU *cpu, struct MMAP *mmap)
{
	// ADC B
	// Add to register A the value in register B and carry
	IGNORE(mmap);
	uint8_t carryValue = compute8BitCarry(cpu->A, cpu->B + cpu->F.C) & 0x01;
	uint8_t halfCarryValue = compute8BitHalfCarry(cpu->A, cpu->B + cpu->F.C) & 0x01;
	cpu->A += cpu->B + cpu->F.C;
	cpu->F.C = carryValue;
	cpu->F.H = halfCarryValue;
	cpu->F.N = 0x0;
	cpu->F.Z = cpu->A == 0x00;
	cpu->PC++;
}

void op0x89(struct CPU *cpu, struct MMAP *mmap)
{
	// ADC C
	// Add to the register A the value in register C and carry
	IGNORE(mmap);
	uint8_t carryValue = compute8BitCarry(cpu->A, cpu->C + cpu->F.C) & 0x01;
	uint8_t halfCarryValue = compute8BitHalfCarry(cpu->A, cpu->C + cpu->F.C) & 0x01;
	cpu->A += cpu->C + cpu->F.C;
	cpu->F.C = carryValue;
	cpu->F.H = halfCarryValue;
	cpu->F.N = 0x0;
	cpu->F.Z = cpu->A == 0x00;
	cpu->PC++;
}

/* 0x9X */

void op0x99(struct CPU *cpu, struct MMAP *mmap)
{
	// SBC C
	// Substracts from register A, the carry and the value in the C register
	IGNORE(mmap);
	uint8_t carryValue = compute8BitBorrowing(cpu->A, cpu->C - cpu->F.C) & 0x01;
	uint8_t halfCarryValue = compute8BitHalfBitBorrowing(cpu->A, cpu->C - cpu->F.C) & 0x01;
	cpu->A = cpu->A - cpu->C - cpu->F.C;
	cpu->F.Z = cpu->A == 0x00;
	cpu->F.N = 1;
	cpu->F.H = halfCarryValue;
	cpu->F.C = carryValue;
	cpu->PC++;
}

void op0x9F(struct CPU *cpu, struct MMAP *mmap)
{
	// SBC A
	// Substracts from register A, the carry and the value in the C register
	IGNORE(mmap);
	uint8_t carryValue = compute8BitBorrowing(cpu->A, cpu->A - cpu->F.C) & 0x01;
	uint8_t halfCarryValue = compute8BitHalfBitBorrowing(cpu->A, cpu->A - cpu->F.C) & 0x01;
	cpu->A = cpu->A - cpu->A - cpu->F.C;
	cpu->F.Z = cpu->A == 0x00;
	cpu->F.N = 1;
	cpu->F.H = halfCarryValue;
	cpu->F.C = carryValue;
	cpu->PC++;
}

/* 0xBX */

void op0xB9(struct CPU *cpu, struct MMAP *mmap)
{
	// CP C
	// Evaluates A - C updating flags but not updating registers
	IGNORE(mmap);
	cpu->F.Z = (cpu->A - cpu->C) == 0x00;
	cpu->F.N = 1;
	cpu->F.H = compute8BitHalfBitBorrowing(cpu->A, cpu->C);
	cpu->F.C = compute8BitBorrowing(cpu->A, cpu->C);
	cpu->PC++;
}

void op0xBB(struct CPU *cpu, struct MMAP *mmap)
{
	// CP E
	// Evaluates A - E updating flags but not updating registers
	IGNORE(mmap);
	cpu->F.Z = (cpu->A - cpu->E) == 0x00;
	cpu->F.N = 1;
	cpu->F.H = compute8BitHalfBitBorrowing(cpu->A, cpu->E);
	cpu->F.C = compute8BitBorrowing(cpu->A, cpu->E);
	cpu->PC++;
}

/* 0xCX */

void op0xCC(struct CPU *cpu, struct MMAP *mmap)
{
	// CALL Z, nn
	// Condition function call
	cpu->PC++;
	// the condition is true if the Zero bit is active
	if (cpu->F.Z == 0x1) {
		uint8_t newPcLsb = readMemory(mmap, cpu->PC++);
		uint8_t newPcMsb = readMemory(mmap, cpu->PC++);
		uint16_t newPC = (((uint16_t) newPcMsb) << 8) | newPcLsb;
		cpu->SP--;
		cpu->PC = newPC;
	}
}

void op0xCE(struct CPU *cpu, struct MMAP *mmap)
{
	// ADC n
	cpu->PC++;
	uint8_t n = readMemory(mmap, cpu->PC);
	cpu->A += n + cpu->F.C;
	cpu->F.Z = cpu->A == 0x0;
	cpu->F.N = 0x0;	
	cpu->F.H = compute8BitHalfCarry(cpu->A, n + cpu->F.C);
	cpu->F.C = compute8BitCarry(cpu->A, n + cpu->F.C);
	cpu->PC++;
}	

/* 0xDX */

void op0xD9(struct CPU *cpu, struct MMAP *mmap)
{
	// RETI
	// Non conditional return from interrupt handler
	// Enables interrupts (IME)
	uint8_t newPcLsb = readMemory(mmap, cpu->SP++);
	uint8_t newPcMsb = readMemory(mmap, cpu->SP++);
	cpu->PC = (((uint16_t) newPcMsb) << 8) | newPcLsb;
	cpu->IME = true;
}

void op0xDC(struct CPU *cpu, struct MMAP *mmap)
{
	// CALL C, nn
	// Conditional jump to nn address if the condition in C is met
	cpu->PC++;
	if (cpu->C == 0x00) {
		uint8_t newPcLsb = readMemory(mmap, cpu->PC++);
		uint8_t newPcMsb = readMemory(mmap, cpu->PC++);
		uint16_t newPC = (((uint16_t) newPcMsb) << 8) | newPcLsb;
		cpu->SP--;
		writeMemory(mmap, cpu->SP--, (cpu->PC & 0xFF00) >> 8);
		writeMemory(mmap, cpu->SP, cpu->PC & 0x00FF);
		cpu->PC = newPC;
	}
}

void op0xDD(struct CPU *cpu, struct MMAP *mmap)
{
	// Undefined
	// HALTs by default as should not end here
	IGNORE(cpu);
	IGNORE(mmap);
}

/* 0xEX */

void op0xE6(struct CPU *cpu, struct MMAP *mmap)
{
	// AND n
	// Bitwise AND between A register and data n
	cpu->PC++;
	uint8_t data = readMemory(mmap, cpu->PC++);
	cpu->A = cpu->A & data;
	cpu->F.Z = cpu->A == 0x00;
	cpu->F.N = 0;
	cpu->F.H = 1;
	cpu->F.C = 0;
}

void op0xEC(struct CPU *cpu, struct MMAP *mmap)
{
	// Undefined opcode
	// HALT by default
	// No operation should end up here
	IGNORE(cpu);
	IGNORE(mmap);
}

void op0xED(struct CPU *cpu, struct MMAP *mmap)
{
	// Undefined opcode
	// HALT by default
	// No operation should end up here
	IGNORE(cpu);
	IGNORE(mmap);
}
