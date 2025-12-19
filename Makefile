TARGET = gb-emu
SRC = memory.c cpu.c ppu.c main.c

CC = gcc

# pkg-config automatically gets SDL2 CFLAGS and LIBS
SDL_CFLAGS = $(shell pkg-config --cflags sdl2)
SDL_LIBS = $(shell pkg-config --libs sdl2)

CFLAGS = -Wall -Wextra -Wpedantic -O3 $(SDL_CFLAGS) -g

LDFLAGS = $(SDL_LIBS) -lm

all: $(TARGET)

$(TARGET): $(SRC)
	@echo "Compiling $(TARGET)..."
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilation successful. Run with: ./$(TARGET)"

clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET)
	@echo "Cleanup complete."

