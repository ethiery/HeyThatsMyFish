#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "server.h"

#ifndef NO_GRAPHICS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define PI 3.14159265

#define SCREEN_FPS 60
#define TICKS_PER_FRAME 1000/SCREEN_FPS

//Screen dimension constants
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;



SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
TTF_Font *sdlFont;
int sdlFontSize;

Uint32 previousTime, currentTime;

typedef struct graphicElement
{
	float x,y,w,h;
	SDL_Texture* sprite;
    SDL_Color color;

}graphicElement;

struct gameGraphics
{
	SDL_Texture* background;
	graphicElement** tiles;
	int nb_tiles;
	graphicElement** penguins;
	int nb_penguins;

	SDL_Texture** tiles_sprites;
	int nb_sprites;

	SDL_Texture* penguin_sprite;

    SDL_Color colors[6];
};

struct gameGraphics* graphics;



int display_init();
void display_free();
int graphics_init(struct graph* g, char* layout);
void graphics_free();
void initColors();
SDL_Texture* loadTexture(char* filepath);
void compute_size(SDL_Texture* texture, float* w, float* h);
void renderTexture(SDL_Texture* texture, float x, float y, float w, float h);
int handleEvents(int* quit);
void drawRectangle(int x1, int y1, int x2, int y2);
void drawLine(int x1, int y1, int x2, int y2);
void drawHexagon(int x, int y, int size);
void drawElement(graphicElement* elem);
void drawGraphics();
void drawEdges(struct graph* g);
void drawIds();
void drawScores();
void draw();
void synchronize(struct graph* g);
void updateScreen();
void compute_geometry(struct graph* g, int tile_id, int* visited, float current_x, float current_y, char* layout);
void align_n_resize();

#endif // for NO_GRAPHICS

#endif
