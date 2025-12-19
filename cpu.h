/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __CPU_H_
#define __CPU_H_

/**
 * The CPU consist of eight 8-bit registers which can be 
 * extended to four 16-bit registers pairs
 * +-------+--------+
 * | 8-bit |-16 bit |
 * +-------+--------+
 * | A - F |   AF   |
 * | B - C |   BC   |
 * | D - E |   DE   |
 * | H - L |   HL   |
 * +----------------+
 */
struct CPU 
{
	// PC: Program Counter
	// SP: Stack Pointer
	unsigned short PC, SP;
	// 8-bit registers
	unsigned char A; 
	unsigned char F; 
	unsigned char B;
	unsigned char C;
	unsigned char D;
	unsigned char E;
	unsigned char H;
	unsigned char L;
	// 16-bit registers
	// they point to the memory location
	// of the related 8-bit registers
	unsigned short *AF; 
	unsigned short *BC;
	unsigned short *DE;
	unsigned short *HL;
};

void initCPU(struct CPU *cpu);

#endif //__CPU_H_
