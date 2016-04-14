/*
	Avoid Electric Death
	by Brian Puthuff

	aed_functions.c
	Modified: Wed Apr 13 23:27:51 PDT 2016
	
	This source code file contains the functions used
	by this program.
*/

#include "aed_functions.h"

/* first time initialization */
void initialize(void)
{
	extern SDL_Rect tiles[];
	extern SDL_Rect corners[];
	extern char map[];
	extern struct player p;
	extern struct player robots[];

	int i;

	/* initialize tiles */
	for(i = 0; i < 16; i++)
	{
		tiles[i].w = tiles[i].h = 32;
		tiles[i + 16].w = tiles[i + 16].h = 32;
		tiles[i + 32].w = tiles[i + 32].h = 32;
		tiles[i].y - 0;
		tiles[i + 16].y = 32;
		tiles[i + 32].y = 64;
		tiles[i].x = tiles[i + 16].x = tiles[i + 32].x = i * 32;

	}

	/* initialize corner tiles */
	for(i = 0; i < 4; i++)
	{
		corners[i].w = corners[i].h = 8;
		corners[i].x = 352 + (i * 8);
		corners[i].y = 64;
	}

	/* initialize player and robots structure vars */
	p.location = 0;
	p.state = 0;
	for(i = 0; i < 5; i++)
	{
		robots[i].location = 0;
		robots[i].state = 0;
	}

	/* reset */
	reset();
}

/* reset game */
void reset(void)
{
	extern char map[];
	extern struct player p;
	extern struct player robots[];

	int i;

	/* generate initial map data */
	generateMap();

	/* bring player and robots to life */
	p.state = 0;
	for(i = 0; i < 5; i++)
		robots[i].state = 0;

	/* set player / robots start position */
	setPlayerStartLocation();
	setRobotsStartLocations();
}

/* function to generate random map layout */
void generateMap(void)
{
	extern char map[];

	int i, v;

	/* initialize basic map */
	for(i = 0; i < CELLS; i++)
	{
		if(rand() % 10 == 5)
			map[i] = 16;
		else
			map[i] = 0;
	}

	/* borders */
	for(i = 0; i < W; i++)
	{
		map[i] = 16;
		map[i + 180] = 16;
	}
	for(i = 1; i < H - 1; i++)
	{
		map[i * W] = 16;
		map[i * W + 19] = 16;
	}

	/* update walls */
	for(i = 0; i < CELLS; i++)
	{
		v = 0;
		if(map[i] == 16)
		{
			if(i > 19)
				if(map[i - 20] > 15)
					v = v + 8;
			if((i % 20) != 0)
				if(map[i - 1] > 15)
					v = v + 4;
			if(((i + 1) % 20) != 0)
				if(map[i + 1] > 15)
					v = v + 2;
			if(i < 180)
				if(map[i + 20] > 15)
					v = v + 1;
			map[i] = v + 16;
		}
	}


	/* shadow maps */
	for(i = 0; i < CELLS; i++)
	{
		if(map[i] == 0)
		{
			v = 0;
			if(map[i - 1] > 15)
				v = v + 1;
			if(map[i - 21] > 15)
				v = v + 2;
			if(map[i - 20] > 15)
				v = v + 4;

			if(v == 7)
				v = 5;
			map[i] = v;
		}
	}

	/*	
	for(i = 0; i < H; i++)
	{
		for(v = 0; v < W; v++)
			printf("%3d", map[i * W + v]);
		printf("\n");
	}
	*/
}

/* select random +++ SAFE +++ start position */
void setPlayerStartLocation(void)
{
	extern char map[];
	extern struct player p;

	do
	{
		p.location = (rand() % 200);
	} while (map[p.location] > 15);	
}

/* select random +++ SAFE +++ start posotions */
void setRobotsStartLocations(void)
{
	extern char map[];
	extern struct player p;
	extern struct player robots[];

	int r, i, j;
	char b;
	for(j = 0; j < 5; j++)
	{
		do
		{
			b = 0; /* init false */
			r = (rand() % 200);
			for(i = j; i > 0; i--)
			{
				if(robots[i - 1].location == r)
					b = 1;
			}
			if(map[r] > 15)
				b = 1;
			if(p.location == r)
				b = 1;
		} while(b == 1);
		robots[j].location = r;
	}
}

/* function tp draw the map to the renderer */
void renderMap(void)
{
	extern SDL_Renderer* renderer;
	extern SDL_Texture* texture;
	extern SDL_Rect tiles[];
	extern SDL_Rect corners[];
	extern char map[];

	int r, c;
	int c0, c1, c2, c3;

	SDL_Rect destination;
	destination.w = destination.h = 32;
	
	for(r = 0; r < H; r++)
		for(c = 0; c < W; c++)
		{
			destination.x = 32 * c;
			destination.y = 32 * r;
			SDL_RenderCopy(renderer, texture, &tiles[map[(r * W) + c]], &destination);
		}

	/* corner fixes */
	destination.w = destination.h = 8;
	for(r = 1; r < (H - 1); r++)
		for(c = 1; c < (W - 1); c++)
		{
			if(map[(r * W) + c] < 7)
			{
				c0 = map[((r - 1) * W) + (c - 1)];
				c1 = map[((r - 1) * W) + (c + 1)];
				c2 = map[((r + 1) * W) + (c - 1)];
				c3 = map[((r + 1) * W) + (c + 1)];

				if((c0 == 19) || (c0 == 27) || (c0 == 23) || (c0 == 31))
				{
					destination.x = (c * 32) - 8;
					destination.y = (r * 32) - 8;
					SDL_RenderCopy(renderer, texture, &corners[0], &destination);
				}
				if((c1 == 21) || (c1 == 29) || (c1 == 23) || (c1 == 31))
				{
					destination.x = (c * 32) + 32;
					destination.y = (r * 32) - 8;
					SDL_RenderCopy(renderer, texture, &corners[1], &destination);
				}
				if((c2 == 26) || (c2 == 27) || (c2 == 30) || (c2 == 31))
				{
					destination.x = (c * 32) - 8;
					destination.y = (r * 32) + 32;
					SDL_RenderCopy(renderer, texture, &corners[2], &destination);
				}
				if((c3 == 28) || (c3 == 29) || (c3 == 30) || (c3 == 31))
				{
					destination.x = (c * 32) + 32;
					destination.y = (r * 32) + 32;
					SDL_RenderCopy(renderer, texture, &corners[3], &destination);
				}
			}

		}
}

/* function to draw the hover frame to the renderer */
void renderHoverTile(struct SDL_Rect* destination)
{
	extern SDL_Renderer* renderer;
	extern SDL_Texture* texture;
	extern SDL_Rect tiles[];

	SDL_RenderCopy(renderer, texture, &tiles[7], destination);	
}

/* function to draw the player to the renderer */
void renderPlayer(void)
{
	extern SDL_Renderer* renderer;
	extern SDL_Texture* texture;
	extern SDL_Rect tiles[];
	extern struct player p;

	int x, y;	
	SDL_Rect destination;

	y = (int) floor(p.location / 20);
	x = (int) (p.location - (y * 20));
	destination.w = destination.h = 32;
	destination.x = x * 32;
	destination.y = y * 32;
	if(p.state == 0)
		SDL_RenderCopy(renderer, texture, &tiles[8], &destination);
	else
		SDL_RenderCopy(renderer, texture, &tiles[9], &destination);
}

/* function to draw the robots to the renderer */
void renderRobots(void)
{
	extern SDL_Renderer* renderer;
	extern SDL_Texture* texture;
	extern SDL_Rect tiles[];
	extern struct player robots[];

	int x, y, i;	
	SDL_Rect destination;

	for(i = 0; i < 5; i++)
	{
		y = (int) floor(robots[i].location / 20);
		x = (int) (robots[i].location - (y * 20));
		destination.w = destination.h = 32;
		destination.x = x * 32;
		destination.y = y * 32;
		if(robots[i].state == 0)
			SDL_RenderCopy(renderer, texture, &tiles[10], &destination);
		else
			SDL_RenderCopy(renderer, texture, &tiles[11], &destination);
	}
}

int getCellNumberFromMouseCoordinates(int x, int y)
{
	return (int) (floor((double) x / 32) + (floor((double) y / 32) * 20));
}

char isHoveringTheNeighborhood(int cell)
{
	extern struct player p;

	if((cell >= p.location - 21) && (cell <= p.location - 19))
		return 1;
	if((cell >= p.location - 1) && (cell <= p.location + 1))
		return 1;
	if((cell >= p.location + 19) && (cell <= p.location + 21))
		return 1;
	return 0;
}

void setHoveringRectanglePosition(struct SDL_Rect* r, int cell)
{
	int x, y;
	y = (int) floor(cell / 20);
	x = (int) (cell - (y * 20));
	r->y = y * 32;
	r->x = x * 32;
}

void movePlayer(int next_location)
{
	extern char map[];
	extern struct player p;
	extern struct player robots[];

	int i;
	p.location = next_location;
	for(i = 0; i < 5; i++)
	{
		if(robots[i].location == p.location)
		{
			p.state = 1;
		}
	}

	if(map[p.location] > 15)
	{
		p.state = 1;
	}
}

void moveRobots(void)
{
	extern char map[];
	extern struct player p;
	extern struct player robots[];

	int i, j, rx, ry, px, py;
	int nx, ny;
	char hit;

	py = (int) floor(p.location / 20);
	px = (int) (p.location - (py * 20));

	for(i = 0; i < 5; i++)
	{
		if(robots[i].state == 0)
		{
			ry = (int) floor(robots[i].location / 20);
			rx = (int) (robots[i].location - (ry * 20));
			if(rx < px)
				nx = rx + 1;
			else if(rx > px)
				nx = rx - 1;
			else nx = rx;
			
			if(ry < py)
				ny = ry + 1;
			else if(ry > py)
				ny = ry - 1;
			else ny = ry;
			robots[i].location = (ny * 20) + nx;
			if(robots[i].location == p.location)
				p.state = 1;
		}
	}

	for(j = 0; j < 5; j++)
	{
		hit = 0;
		for(i = j + 1; i > 0; i--)
		{
			if(robots[j].state != 1)
				if(robots[i].state != 1)
					if(i != j)
						if(robots[j].location == robots[i].location)
						{
							robots[i].state = 1;
							hit = 1;
						}

		}
		if(hit == 1)
			robots[j].state = 1;
	}

	for(i = 0; i < 5; i++)
	{
		if(robots[i].state == 0)
			if(map[robots[i].location] > 15)
				robots[i].state = 1;
	}
}


