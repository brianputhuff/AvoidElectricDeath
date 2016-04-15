/*
	Avoid Electric Death
	by Brian Puthuff

	aed_variables.h
	Modified: Wed Apr 13 22:33:48 PDT 2016
	
	This header file contains the global variables used
	by this program.
*/

#ifndef AED_VARIABLES_H
#define AED_VARIABLES_H

#include "SDL2/SDL.h"

#define WIDTH 640
#define HEIGHT 672
#define CELLS 400
#define ROBOTCOUNT 20
#define DIM 20

/* rendering pointers */
SDL_Renderer* renderer;
SDL_Texture* texture;

/* tile collections */
SDL_Rect tiles[48];
SDL_Rect corners[4];

/* map data */
char map[CELLS];

/* stats */
char stats[4];

/* player struct */
struct player
{
	int location;
	char state;
} p;

/* robot struct array */
struct player robots[ROBOTCOUNT];

#endif


