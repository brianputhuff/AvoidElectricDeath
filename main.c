#include <stdio.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "aed_functions.h"
#include "aed_variables.h"

/*
	Avoid Electric Death!
	by Brian Puthuff

	main.c
	Updated: Wed Apr 13 23:31:00 PDT 2016
*/


enum {FALSE = 0, TRUE = 1};

int main(int argc, char** argv)
{

	/* external variables */
	extern SDL_Renderer* renderer;
	extern SDL_Texture* texture;
	extern char map[];
	extern struct player p;	
	extern struct player robots[];

	SDL_Rect hover_position;
	
	/* internal variables */
	SDL_Window* window;
	SDL_Surface* load_surface;
	Uint32 color_key;
	char is_playing;
	char is_running;
	char is_hovering;
	char invalid_move;
	SDL_Event event;
	int c, i;
	
	/* initialize SDL */
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("%s\n", SDL_GetError());
		return 1;
	}

	/* create window and renderer */
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
	if((window == NULL) || (renderer == NULL))
	{
		printf("%s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	/* set window title */
	SDL_SetWindowTitle(window, "Avoid Electric Death!");

	/* initial renderer update */
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	/* load tile set */
	load_surface = SDL_LoadBMP("./tiles.bmp");
	if(load_surface == NULL) 
	{
		printf("%s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	color_key = SDL_MapRGB(load_surface->format, 0x00, 0xff, 0xff);
	SDL_SetColorKey(load_surface, SDL_TRUE, color_key);
	texture = SDL_CreateTextureFromSurface(renderer, load_surface);
	SDL_FreeSurface(load_surface); /* regardless if it worked or not, we free this memory */
	if(texture == NULL)
	{
		printf("%s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/* initialize */
	srand(time(NULL));
	initialize();

	/* initialize game */
	is_running = TRUE;
	is_playing = TRUE;
	hover_position.w = hover_position.h = 32;
	hover_position.x = hover_position.y = 0;
	is_hovering = FALSE;
	
	/* main game loop */
	while(is_running == TRUE)
	{
		/* render */
		renderMap();
		renderRobots();
		renderPlayer();
		if(is_hovering == TRUE)
			renderHoverTile(&hover_position);
		renderStats();
		SDL_RenderPresent(renderer);
		
		invalid_move = FALSE;

		/* check for input */
		while(SDL_PollEvent(&event) > 0)
		{
			switch(event.type)
			{
				case SDL_QUIT:
					is_running = FALSE;
					break;

				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_F9)
					{
						/* reset game */
						reset();
						is_playing = TRUE;
						is_hovering = FALSE;
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					if(is_playing == TRUE)
					{
						c = getCellNumberFromMouseCoordinates(event.motion.x, event.motion.y);
						if(isHoveringTheNeighborhood(c) == TRUE)
						{
							is_hovering = FALSE;

							/* if button one, move */
							if(event.button.button == SDL_BUTTON_LEFT)
								movePlayer(c);
							else
								if(c == p.location)
									movePlayer(rand() % 200);
								else
									invalid_move = TRUE;

							if(p.state == 1)
								is_playing = FALSE;
							else
								if(invalid_move == FALSE)
								{
									moveRobots();
									if(p.state == 1)
										is_playing = FALSE;
									/* check for robot deaths */
								}
						}
					}
					break;

				case SDL_MOUSEMOTION:
					if(is_playing == TRUE)
					{
						c = getCellNumberFromMouseCoordinates(event.motion.x, event.motion.y);
						if(isHoveringTheNeighborhood(c) == TRUE)
						{
							is_hovering = TRUE;
							setHoveringRectanglePosition(&hover_position, c);
						}
						else
							is_hovering = FALSE;
					}
					break;
			}
		}
	}

	/* free up memory before exit */
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}


