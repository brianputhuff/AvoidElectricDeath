/*
	Avoid Electric Death
	by Brian Puthuff

	aed_functions.h
	Modified: Wed Apr 13 Wed Apr 13 23:30:24 PDT 2016

	This header code file provides function
	prototypes for functions used by this
	program.
*/

#ifndef AED_FUNCTIONS_H
#define AED_FUNCTIONS_H

#include <stdlib.h>
#include "SDL2/SDL.h"
#include "aed_variables.h"
#define CELLS	200
#define W	20
#define H	10

/* initializers */
void initialize(void);
void reset(void);
void generateMap(void);
void setPlayerStartLocation(void);
void setRobotsStartLocations(void);

/* rendering */
void renderMap(void);
void renderHoverTile(struct SDL_Rect* destination);
void renderPlayer(void);
void renderRobots(void);

/* functions */
int getCellNumberFromMouseCoordinates(int x, int y);
char isHoveringTheNeighborhood(int cell);
void setHoveringRectanglePosition(struct SDL_Rect* r, int cell);
void movePlayer(int next_location);
void moveRobots(void);

#endif
