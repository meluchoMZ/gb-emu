/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "ppu.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

bool initPPU(struct PPU *ppu, FILE *logFile)
{
	if (logFile == NULL) {
		fprintf(stderr, "[PPU] Could not initialize PPU: logFile is NULL");
		return false;
	}

	if (ppu == NULL) {
		fprintf(logFile, "[PPU] Could not initialize PPU: ppu is NULL");
		return false;
	}

	int windowFlags = 0;
	int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	int computedWidth = SCREEN_WIDTH * SCALE_FACTOR;
	int computedHeight = SCREEN_HEIGHT * SCALE_FACTOR;

	// initialize all the ppu fields
	ppu->logFile = logFile;
	ppu->frameCounter = 0;
	ppu->fps = 0;

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		fprintf(ppu->logFile, "Error initializing SDL subsystem: %s\n", SDL_GetError());
		return false;
	}

	ppu->window = SDL_CreateWindow("GBemu",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH * SCALE_FACTOR, SCREEN_HEIGHT * SCALE_FACTOR, 
			windowFlags);

	if (ppu->window == NULL) {
		fprintf(ppu->logFile, "Error creating SDL window: %s\n", SDL_GetError());
		return false;
	}

	ppu->renderer = SDL_CreateRenderer(ppu->window, -1, rendererFlags);

	if (ppu->renderer == NULL) {
		fprintf(ppu->logFile, "Error creating SDL renderer: %s\n", SDL_GetError());
		return false;
	}

	fprintf(ppu->logFile, "Initializing pixel buffer with size %ld:\n", sizeof(uint32_t[computedWidth][computedHeight]));
	ppu->pixelBuffer = malloc(computedWidth * computedHeight * sizeof(uint32_t));
	for (int i = 0; i < computedWidth; ++i) {
		for (int j = 0; j < computedHeight; ++j) {
			if (i < computedWidth/2 && j < computedHeight/2)
				ppu->pixelBuffer[computedWidth * j + i] = TILE_WHITE;
			if (i < computedWidth/2 && j > computedHeight/2)
				ppu->pixelBuffer[computedWidth * j + i] = TILE_DARK_GRAY;
			if (i > computedWidth/2 && j < computedHeight/2)
				ppu->pixelBuffer[computedWidth * j + i] = TILE_LIGHT_GRAY;
			if (i > computedWidth/2 && j > computedHeight/2)
				ppu->pixelBuffer[computedWidth * j + i] = TILE_BLACK;
		}
	}

	ppu->debugWindow = SDL_CreateWindow("Tile VRAM Debug",
			// offset by 20 pixels to the left of the emulator window
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			TILE_DEBUG_WINDOW_WIDTH * SCALE_FACTOR,
			TILE_DEBUG_WINDOW_HEIGHT * SCALE_FACTOR, 
			0);

	if (ppu->debugWindow == NULL) {
		fprintf(ppu->logFile, "Error creating SDL window (debug): %s\n", SDL_GetError());
		return false;
	}

	ppu->debugRenderer = SDL_CreateRenderer(ppu->debugWindow, -1, rendererFlags);

	if (ppu->debugRenderer == NULL) {
		fprintf(ppu->logFile, "Error creating SDL renderer (debug): %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void finalizePPU(struct PPU *ppu) 
{
	if (ppu == NULL) {
		SDL_Quit();
	}

	free(ppu->pixelBuffer);
	
	if (ppu->renderer != NULL) {
		SDL_DestroyRenderer(ppu->renderer);
	}

	if (ppu->window != NULL) {
		SDL_DestroyWindow(ppu->window);
	}

	if (ppu->debugRenderer != NULL) {
		SDL_DestroyRenderer(ppu->debugRenderer);
	}

	if (ppu->debugWindow != NULL) {
		SDL_DestroyWindow(ppu->debugWindow);
	}

	SDL_Quit();
}

void updateFramesPerSecond(struct PPU *ppu, const char * gameTitle)
{
	Uint64 currentFrameCounter = SDL_GetTicks();
	ppu->fps++;
	if (currentFrameCounter > ppu->frameCounter + 1000) {
		char newWindowTitle[16] = {'\0'};
		sprintf(newWindowTitle, "%s - FPS: %d", gameTitle, ppu->fps);
		SDL_SetWindowTitle(ppu->window, newWindowTitle); 
		ppu->frameCounter = currentFrameCounter;
		ppu->fps = 0;
	}
	return;
}

void renderFrame(struct PPU *ppu, const char *gameTitle)
{
	int texturePitch = 0;
	void *texturePixels = NULL;
	// We have to manage textures in order to bulk copy our pixelBuffer, 
	// the one where the CPU moves and writes video data, into our 
	// rendering library. We do it in bulk in order to avoid pixel by pixel
	// rendering as this has a lot of performance drawbacks
	SDL_Texture *texture = SDL_CreateTexture(ppu->renderer, 
			SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
			SCREEN_WIDTH*SCALE_FACTOR,
			SCREEN_HEIGHT*SCALE_FACTOR);
	if (texture == NULL) {
		fprintf(ppu->logFile, "Error creating texture: %s\n", SDL_GetError());
		return;
	}
	if (SDL_LockTexture(texture, NULL, &texturePixels, &texturePitch) == 0) {
		// texturePitch holds the lenght of a row of pixels in bytes, which is 
		// SCREEN_WIDTH * sizeof(uint32_t) = SCREEN_WIDTH * 4
		memcpy(texturePixels, ppu->pixelBuffer, texturePitch * SCREEN_HEIGHT*SCALE_FACTOR);
	} else {
		fprintf(ppu->logFile, "Unable to lock texture: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		return;
	}
	SDL_UnlockTexture(texture);

	/**
	 * The rendering process is divided in 3 stages:
	 *  - RenderClear: clears the renderer before writing new data
	 *  - RenderCopy: copies the pixelBuffer data into the renderer buffer 
	 *  - RenderPresent: flushes the renderer data to the GPU to be rendered in the screen
	 */
	SDL_RenderClear(ppu->renderer);
	SDL_RenderCopy(ppu->renderer, texture, NULL, NULL);
	SDL_RenderPresent(ppu->renderer);

	SDL_RenderClear(ppu->debugRenderer);
	SDL_RenderPresent(ppu->debugRenderer);

	SDL_DestroyTexture(texture);
	// variables to store current emulator window
	int emulatorWindowPositionX;
	int emulatorWindowPositionY;
	SDL_GetWindowPosition(ppu->window, &emulatorWindowPositionX, &emulatorWindowPositionY);
	SDL_SetWindowPosition(ppu->debugWindow, 
			emulatorWindowPositionX + (SCREEN_WIDTH * SCALE_FACTOR) + 20, emulatorWindowPositionY);

	updateFramesPerSecond(ppu, gameTitle);
	return;
}

bool proccessInput()
{
	SDL_Event sdlEvent;
	SDL_PollEvent(&sdlEvent);
	switch (sdlEvent.type)
	{
		case SDL_QUIT: 
			return true;
			break;

		default:
			break;
	}
	return false; 
}
