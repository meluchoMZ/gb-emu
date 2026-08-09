TARGET = gb-emu
SRC = cartridge.c io.c memory.c cpu_instructions.c cpu.c ppu.c boot.c main.c

CC = gcc

# pkg-config automatically gets SDL2 CFLAGS and LIBS
SDL_CFLAGS = $(shell pkg-config --cflags sdl2)
SDL_LIBS = $(shell pkg-config --libs sdl2)

CFLAGS = -Wall -Wextra -Werror -Wpedantic $(SDL_CFLAGS) -g

LDFLAGS = $(SDL_LIBS) -lm

all: $(TARGET)

$(TARGET): $(SRC)
	@echo "Compiling $(TARGET)..."
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC)
	@echo "Compilation successful. Run with: ./$(TARGET) <game-ROM>"

clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET)
	@echo "Cleanup complete."

