/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "cpu.h"

void initCPU(struct CPU *cpu)
{
	// since the data in the struct CPU is aligned,
	// 8-bit registers are contiguous in
	// memory so writing in 16-bits registers
	// will write a byte on each single register
	cpu->AF =(unsigned short *) &(cpu->A);
	cpu->BC =(unsigned short *) &(cpu->B);
	cpu->DE =(unsigned short *) &(cpu->D);
	cpu->HL =(unsigned short *) &(cpu->H);
}

