/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __CPU_INSTRUCTIONS_H_
#define __CPU_INSTRUCTIONS_H_

#include "cpu.h"
#include "memory.h"

/* 0x0X */
void op0x00(struct CPU *cpu, struct MMAP *mmap);
void op0x03(struct CPU *cpu, struct MMAP *mmap);
void op0x06(struct CPU *cpu, struct MMAP *mmap);
void op0x08(struct CPU *cpu, struct MMAP *mmap);
void op0x0B(struct CPU *cpu, struct MMAP *mmap);
void op0x0C(struct CPU *cpu, struct MMAP *mmap);
void op0x0D(struct CPU *cpu, struct MMAP *mmap);
void op0x0E(struct CPU *cpu, struct MMAP *mmap);

/* 0x1X */
void op0x10(struct CPU *cpu, struct MMAP *mmap);
void op0x11(struct CPU *cpu, struct MMAP *mmap);
void op0x17(struct CPU *cpu, struct MMAP *mmap);
void op0x1A(struct CPU *cpu, struct MMAP *mmap);
void op0x1F(struct CPU *cpu, struct MMAP *mmap);

/* 0x2X */
void op0x20(struct CPU *cpu, struct MMAP *mmap);
void op0x21(struct CPU *cpu, struct MMAP *mmap);
void op0x26(struct CPU *cpu, struct MMAP *mmap);

/* 0x3X */
void op0x31(struct CPU *cpu, struct MMAP *mmap);
void op0x32(struct CPU *cpu, struct MMAP *mmap);
void op0x33(struct CPU *cpu, struct MMAP *mmap);
void op0x3E(struct CPU *cpu, struct MMAP *mmap);

/* 0x4X */
void op0x4F(struct CPU *cpu, struct MMAP *mmap);

/* 0x6X */
void op0x63(struct CPU *cpu, struct MMAP *mmap);
void op0x66(struct CPU *cpu, struct MMAP *mmap);
void op0x67(struct CPU *cpu, struct MMAP *mmap);
void op0x6E(struct CPU *cpu, struct MMAP *mmap);

/* 0x7X */
void op0x73(struct CPU *cpu, struct MMAP *mmap);
void op0x77(struct CPU *cpu, struct MMAP *mmap);

/* 0x8X */
void op0x83(struct CPU *cpu, struct MMAP *mmap);
void op0x88(struct CPU *cpu, struct MMAP *mmap);
void op0x89(struct CPU *cpu, struct MMAP *mmap);

/* 0x9X */
void op0x99(struct CPU *cpu, struct MMAP *mmap);
void op0x9F(struct CPU *cpu, struct MMAP *mmap);

/* 0xAX */
void op0xAF(struct CPU *cpu, struct MMAP *mmap);

/* 0xBX */
void op0xB9(struct CPU *cpu, struct MMAP *mmap);
void op0xBB(struct CPU *cpu, struct MMAP *mmap);

/* 0xCX */
void op0xC1(struct CPU *cpu, struct MMAP *mmap);
void op0xC5(struct CPU *cpu, struct MMAP *mmap);
void op0xCB(struct CPU *cpu, struct MMAP *mmap);
void op0xCC(struct CPU *cpu, struct MMAP *mmap);
void op0xCD(struct CPU *cpu, struct MMAP *mmap);
void op0xCE(struct CPU *cpu, struct MMAP *mmap);

/* 0xDX */
void op0xD9(struct CPU *cpu, struct MMAP *mmap);
void op0xDC(struct CPU *cpu, struct MMAP *mmap);
void op0xDD(struct CPU *cpu, struct MMAP *mmap);

/* 0xEX */
void op0xE0(struct CPU *cpu, struct MMAP *mmap);
void op0xE2(struct CPU *cpu, struct MMAP *mmap);
void op0xE6(struct CPU *cpu, struct MMAP *mmap);
void op0xEC(struct CPU *cpu, struct MMAP *mmap);
void op0xED(struct CPU *cpu, struct MMAP *mmap);

/* CB prefixed */

/* CB 0x1X */
void opCB0x11(struct CPU *cpu, struct MMAP *mmap);

/* CB 0x7X */
void opCB0x7C(struct CPU *cpu, struct MMAP *mmap);

/** 
 * Executes a single CPU instruction. The execution consists of:
 *  - read the instruction pointed by the program counter
 *  - execute the operation in the CPU
 *  - logs which operation is being executed 
 * @param cpu a pointer to a struct CPU
 * @param mmap a pointer to a struct MMAP
 */
void executeCPUInstruction(struct CPU *cpu, struct MMAP *mmap);

#endif //__CPU_INSTRUCTIONS_H_
