/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __PPU_H_
#define __PPU_H_

#include <SDL2/SDL.h>
#include <stdio.h>

// Screen properties
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCALE_FACTOR 4 // scale it up to 4 on testing

// Tile color code
#define TILE_WHITE      0xFFFFFFFF
#define TILE_LIGHT_GRAY 0xFFAAAAAA
#define TILE_DARK_GRAY  0xFF555555
#define TILE_BLACK      0xFF000000

/**
 * Defines the PixelProccessingUnit
 * @Attribute window
 * @Attribute renderer
 * @Attribute logFile
 * @Attribute pixelBuffer
 *
 * pixelBuffer elements have 4 bytes due to how the 
 * RGBA32 format works:
 *  - each pixel stores a Red, Green, Blue and Alpha 
 *  component, 8 bits each
 */
struct PPU
{
	SDL_Window   *window;
	SDL_Renderer *renderer;
	FILE         *logFile;
	uint32_t     *pixelBuffer;
	Uint64        frameCounter;
	unsigned int  fps;
};

/**
 * Initializes all PPU subsystems
 * @param ppu a PPU pointer to the logFile
 * @return 1 if error, 0 if success
 */
int initPPU(struct PPU *ppu, FILE *logFile);

/**
 * Renders a frame
 * @param ppu a pointer to the ppu 
 */
void renderFrame(struct PPU *ppu);


/**
 * Ends the PPU execution liberating all its resources
 * @param ppu a pointer to a non null ppu 
 */
void finalizePPU(struct PPU *ppu);

bool proccessInput();

#endif // __PPU_H_
