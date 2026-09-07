.PHONY: all test clean

TARGET = gb
TEST_TARGET = gbtest

SRC_ALU = src/soc/cpu.c src/soc/arith_utils.c src/soc/cpu_instructions.c
SRC_MEMORY = src/soc/memory.c
SRC_PERIPHERALS = src/peripherals/cartridge.c
SRC_VIDEO = src/video/ppu.c
SRC = src/io.c $(SRC_PERIPHERALS) $(SRC_VIDEO) $(SRC_MEMORY) $(SRC_ALU) src/boot.c src/main.c

TEST_SRC = src/soc/arith_utils.c test/soc/arith_utils.c

CC = gcc

# pkg-config automatically gets SDL2 CFLAGS and LIBS
SDL_CFLAGS = $(shell pkg-config --cflags sdl2)
SDL_LIBS = $(shell pkg-config --libs sdl2)

CFLAGS = -Wall -Wextra -Werror -Wpedantic -fsanitize=address $(SDL_CFLAGS) -g

LDFLAGS = $(SDL_LIBS) -lm -lctest


$(TARGET): $(SRC)
	@echo "Compiling $(TARGET)..."
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC)
	@echo "Compilation successful. Run with: ./$(TARGET) <game-ROM>"

test: $(TEST_SRC)
	@echo "Compiling $(TEST_TARGET)..."
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC) $(LDFLAGS) && ./gbtest

all: $(TARGET) $(TEST_TARGET)

clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET) $(TEST_TARGET)
	@echo "Cleanup complete."

