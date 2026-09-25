/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "arith_utils.h"
#include "bin_utils.h"
#include "cpu.h"
#include "cpu_instructions.h"
#include "memory.h"

#include <stdint.h>

// Tells the compiler to ignore unused parameters
#define IGNORE(x) (void)(x)

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

void op0x06(struct CPU *cpu, struct MMAP *mmap)
{
	// LD B, n
	// Loads the data n into B register
	cpu->PC++;
	uint8_t n = readMemory(mmap, cpu->PC++);
	cpu->B = n;
}

void op0x08(struct CPU *cpu, struct MMAP *mmap)
{
	// LD (nn), SP
	// Loads to the address nn the data from the SP
	cpu->PC++;
	uint8_t addressLsb = readMemory(mmap, cpu->PC++);
	uint8_t addressMsb = readMemory(mmap, cpu->PC++);
	// write Least Significant Bits
	uint16_t targetAddress = toUint16(addressMsb, addressLsb);
	writeMemory(mmap, targetAddress++, lsb(cpu->SP));
	// write Most Significant Bits
	writeMemory(mmap, targetAddress, msb(cpu->SP));
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
	uint16_t data = toUint16(dataMsb, dataLsb);
	cpu->DE = data;
}

void op0x17(struct CPU *cpu, struct MMAP *mmap)
{
	// RLA
	// Rotates the accumulator left through the carry flag
	IGNORE(mmap);
	cpu->A = cpu->A << 1;
	cpu->A = cpu->A | (cpu->F.C & 0x01);
	cpu->F.Z = 0;
	cpu->F.N = 0;
	cpu->F.H = 0;
	cpu->F.C = (cpu->A >> 8) & 0x01;
	cpu->PC++;
}

void op0x1A(struct CPU *cpu, struct MMAP *mmap)
{
	// LD A, (DE)
	// Load into A register the data pointed by DE register
	uint8_t data = readMemory(mmap, cpu->DE);
	cpu->A = data;
	cpu->PC++;
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

/* 0x2X */

void op0x20(struct CPU *cpu, struct MMAP *mmap)
{
	// JR NZ, e
	// Conditional relative jump to e if NZ is met
	cpu->PC++;
	int8_t offset = readMemory(mmap, cpu->PC++);
	if (!cpu->F.Z) {
		cpu->PC = cpu->PC + offset;
	}
}

void op0x21(struct CPU *cpu, struct MMAP *mmap)
{
	// LD HL, nn
	// Loads the data nn into HL register
	cpu->PC++;
	uint8_t lsbData = readMemory(mmap, cpu->PC++);
	uint8_t msbData = readMemory(mmap, cpu->PC++);
	cpu->HL = toUint16(msbData, lsbData);
}

void op0x26(struct CPU *cpu, struct MMAP *mmap)
{
	// LD H, n
	// Loads the data n into H register
	cpu->PC++;
	uint8_t data = readMemory(mmap, cpu->PC++);
	cpu->H = data;
}

/* 0x3X */

void op0x31(struct CPU *cpu, struct MMAP *mmap)
{
	// LD SP, nn
	// Loads 16 bits data into Stack Pointer
	cpu->PC++;
	uint8_t newSpLsb = readMemory(mmap, cpu->PC++);
	uint8_t newSpMsb = readMemory(mmap, cpu->PC++);
	cpu->SP = toUint16(newSpMsb, newSpLsb);
}

void op0x32(struct CPU *cpu, struct MMAP *mmap)
{
	// LD (HL-), A
	// Loads the data from A register to the address HL
	// HL value is then decremented after memory write
	writeMemory(mmap, cpu->HL, cpu->A);
	cpu->HL--;
	cpu->PC++;
}

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

/* 0x4X */

void op0x4F(struct CPU *cpu, struct MMAP *mmap)
{
	// LD C, A
	// Loads the data from A register into C register 
	IGNORE(mmap);
	cpu->C = cpu->A;
	cpu->PC++;
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

void op0x77(struct CPU *cpu, struct MMAP *mmap)
{
	// LD HL, A
	// Loads the data A into memory at address pointed by HL
	writeMemory(mmap, cpu->HL, cpu->A);
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

/* 0xAX */

void op0xAF(struct CPU *cpu, struct MMAP *mmap)
{
	// XOR A
	// XOR between A register and A register and updates A register
	IGNORE(mmap);
	cpu->A = cpu->A ^ cpu->A;
	cpu->F.Z = cpu->A == 0;
	cpu->F.N = 0;
	cpu->F.H = 0;
	cpu->F.C = 0;
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

void op0xC1(struct CPU *cpu, struct MMAP *mmap)
{
	// RET NZ
	// Conditional return if NZ is met
	if (!cpu->F.Z) {
		uint8_t lsb = readMemory(mmap, cpu->SP++);
		uint8_t msb = readMemory(mmap, cpu->SP++);
		cpu->PC = (((uint16_t) msb) << 8) | lsb;
	} else {
		cpu->PC++;
	}
}

void op0xC5(struct CPU *cpu, struct MMAP *mmap)
{
	// PUSH BC
	// Pushes data from BC to the stack memory
	cpu->SP--;
	writeMemory(mmap, cpu->SP--, (cpu->BC & 0xFF00) >> 8);
	writeMemory(mmap, cpu->SP, cpu->BC & 0x00FF);
	cpu->PC++;
}

void op0xCB(struct CPU *cpu, struct MMAP *mmap)
{
	// CB prefix, enables CB instruction set
	IGNORE(mmap);
	cpu->cbPrefix = true;
	cpu->PC++;
}

void op0xCC(struct CPU *cpu, struct MMAP *mmap)
{
	// CALL Z, nn
	// Condition function call
	cpu->PC++;
	// the immediate operand is always processed
	uint8_t newPcLsb = readMemory(mmap, cpu->PC++);
	uint8_t newPcMsb = readMemory(mmap, cpu->PC++);
	// the condition is true if the Zero bit is active
	if (cpu->F.Z) {
		uint16_t newPC = (((uint16_t) newPcMsb) << 8) | newPcLsb;
		cpu->SP--;
		cpu->PC = newPC;
	}
}

void op0xCD(struct CPU *cpu, struct MMAP *mmap)
{
	// CALL nn
	// Non condition jump to immediate nn
	cpu->PC++;
	uint8_t newPcLsb = readMemory(mmap, cpu->PC++);
	uint8_t newPcMsb = readMemory(mmap, cpu->PC++);
	cpu->SP--;
	writeMemory(mmap, cpu->SP--, newPcMsb);
	writeMemory(mmap, cpu->SP, newPcLsb);
	cpu->PC = (((uint16_t) newPcMsb) << 8) | newPcLsb;
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

void op0xE0(struct CPU *cpu, struct MMAP *mmap)
{
	// LDH (n), A
	// Load from accumulator to 0xFF00 + n
	cpu->PC++;
	uint8_t n = readMemory(mmap, cpu->PC++);
	writeMemory(mmap, toUint16(0xFF, n), cpu->A);
}

void op0xE1(struct CPU *cpu, struct MMAP *mmap)
{
	// POP HL
	// Pops data from the stack memory to HL register
	uint8_t lsb = readMemory(mmap, cpu->SP++);
	uint8_t msb = readMemory(mmap, cpu->SP++);
	cpu->HL = toUint16(msb, lsb);
	cpu->PC++;
}

void op0xE2(struct CPU *cpu, struct MMAP *mmap)
{
	// LDH (C), A
	// Loads into memory the value from A into 0xFF00 + C offset
	cpu->PC++;
	uint8_t offset = readMemory(mmap, cpu->PC++);
	writeMemory(mmap, toUint16(0xFF, offset), cpu->A);
}

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

/* CB prefixed */

/* CB 0x1X */

void opCB0x11(struct CPU *cpu, struct MMAP *mmap)
{
	// RL C
	// Rotates the C register left througt the carry flag
	IGNORE(mmap);
	cpu->C = cpu->C << 1;
	cpu->C = cpu->C | (cpu->F.C & 0x01);
	cpu->F.Z = cpu->C == 0x00;
	cpu->F.N = 0;
	cpu->F.H = 0;
	cpu->F.C = (cpu->F.C >> 8) & 0x01;
	cpu->PC++;
}

/* CB 0x7X */

void opCB0x7C(struct CPU *cpu, struct MMAP *mmap)
{
	// BIT 7, H
	// Tests bit 7 of register H
	// The zero flag is set to 1 if the bit 7 is zero
	IGNORE(mmap);
	cpu->F.Z = (cpu->H & 0x80) == 0;
	cpu->F.Z = 0;
	cpu->F.H = 0;
	cpu->PC++;
}

void executeCPUInstruction(struct CPU *cpu, struct MMAP *mmap)
{
	uint8_t instruction = readMemory(mmap, cpu->PC);
	uint8_t rowId = (instruction >> 4) & 0x0F;
	uint8_t columnId = instruction & 0x0F;
	if (cpu->cbPrefix) {
		cpu->cbPrefix = false;
		fprintf(cpu->logFile, "[PC: %02X] Executing CB prefixed instruction %02X, [%X][%X]\n", 
				cpu->PC, instruction, rowId, columnId);
		cpu->extendedInstructionSet[rowId][columnId](cpu, mmap);	
	} else {
		fprintf(cpu->logFile, "[PC: %02X] Executing instruction %02X, [%X][%X]\n", 
				cpu->PC, instruction, rowId, columnId);
		cpu->instructionSet[rowId][columnId](cpu, mmap);
	}
}
