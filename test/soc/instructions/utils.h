/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../../src/soc/cpu.h"
#include "../../../src/soc/cpu_instructions.h"
#include "../../../src/soc/memory.h"

#include <ctest_api.h>
#include <stdbool.h>

// Macro expansions to reduce test developing time

// ignore variable in compilation time;
#define IGNORE(x) (void) x;

/**
 * Creates cpu, (cpu) expected, mmap, logFile variables and initializes them
 */
#define INIT_ENV \
	struct CPU cpu; \
	struct CPU expected; \
	struct MMAP mmap; \
	FILE *logFile = stdout; \
	IGNORE(expected) \
	ASSERT_TRUE(initEnvironment(&cpu, &mmap, logFile));

/**
 * Clones the cpu and executes an instruction
 */
#define EXEC(row, column) \
	ASSERT_TRUE(cloneCPU(&cpu, &expected)); \
	cpu.instructionSet[row][column](&cpu, &mmap);

/**
 * Initializes test environment by initializing cpu and mmap objects
 */
bool initEnvironment(struct CPU *cpu, struct MMAP *mmap, FILE *logFile);

/**
 * Clones the cpu for testing
 */
bool cloneCPU(struct CPU *original, struct CPU *cloned);

/**
 * Checks all cpu registers and flags are equal
 */
void assertCPUEquals(struct CPU expected, struct CPU actual);
