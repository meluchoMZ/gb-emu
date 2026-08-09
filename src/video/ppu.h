/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#ifndef __PPU_H_
#define __PPU_H_

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

// Screen properties
#define SCREEN_BUFFER_WIDTH 256
#define SCREEN_BUFFER_HEIGHT 256
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCALE_FACTOR 1 // scale it up to 4 on testing

// Tile color code
#define TILE_WHITE      0xFFFFFFFF
#define TILE_LIGHT_GRAY 0xFFAAAAAA
#define TILE_DARK_GRAY  0xFF555555
#define TILE_BLACK      0xFF000000

#define TILE_WIDTH 8

// There are 2 32x32 tile maps
// 256x256 pixels each
#define TILE_DEBUG_WINDOW_WIDTH 256
#define TILE_DEBUG_WINDOW_HEIGHT 512

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
	uint16_t      fps;
	uint8_t       scrollX;
	uint8_t       scrollY;
	uint8_t       windowPosX;
	uint8_t       windowPosY;
	uint8_t       lcdcRegister;
	uint32_t     *pixelBuffer;
	FILE         *logFile;
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Window   *debugWindow;
	SDL_Renderer *debugRenderer;
	Uint64        frameCounter;
};

/**
 * Defines each one of the object attribute entries of the 
 * Object Attribute Memory (OAM), located at $FE00-FE9F
 */
struct AttributeEntry
{
	// Vertical position
	uint8_t yPosition;
	// Horizontal position
	uint8_t xPosition;
	// Tile index
	uint8_t tileIndex;
	/**
	 * Attribute flags
	 * +----------+--------+--------+-------------+------+-------------+
	 * |     7    |    6   |    5   |      4      |   3  |   2  -  0   |
	 * +----------+--------+--------+-------------+------+-------------+
	 * | Priority | Y flip | X flip | DMG palette | Bank | CGB palette |
	 * +----------+--------+--------+-------------+------+-------------+
	 */
	struct 
	{
		uint8_t cgbPalette : 3;
		uint8_t bank : 1;
		uint8_t dmgPalette : 1;
		uint8_t xFlip : 1;
		uint8_t yFlip : 1;
		uint8_t priority: 1;
	} flags;
};

/**
 * 64 pixels tile
 */
typedef uint8_t Tile[8][8];


/**
 * Initializes all PPU subsystems
 * @param ppu a pointer to a struct PPU
 * @param logFile a FILE pointer to the desired log file
 */
bool initPPU(struct PPU *ppu, FILE *logFile);

/**
 * Renders the two tile maps for debuggin purposes
 * @param ppu a pointer to a struct PPU
 */
void renderDebugTileInfo(struct PPU *ppu);

uint16_t decode2BPPGraphics(const uint8_t highByte, const uint8_t lowByte);

/**
 * Renders a frame
 * @param ppu a pointer to a struct PPU
 */
void renderFrame(struct PPU *ppu, const char *gameTitle);


/**
 * Ends the PPU execution liberating all its resources
 * @param ppu a pointer to a struct PPU
 */
void finalizePPU(struct PPU *ppu);

bool proccessInput();

#endif // __PPU_H_
