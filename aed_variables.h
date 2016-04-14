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

/* rendering pointers */
SDL_Renderer* renderer;
SDL_Texture* texture;

/* tile collections */
SDL_Rect tiles[48];
SDL_Rect corners[4];

/* map data */
char map[200];

/* player struct */
struct player
{
	int location;
	char state;
} p;

/* robot struct array */
struct player robots[5];

#endif


