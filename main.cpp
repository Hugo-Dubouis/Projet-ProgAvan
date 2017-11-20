#include <SDL2/SDL.h>
#include "game.hpp"
#include "snake.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

///////////////////////////////////////////////////////
// Map settings

// Map width and height
int map_width = 20; // width
int map_height = 21; // height
int dim_allocated = 0;

// Blocs dimensions
const int tile_width = 30;
const int tile_height = 30;

///////////////////////////////////////////////////////
// Main program

int main(int argc, char **argv) {
	// Setup SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "Error while initializing SDL" << std::endl;
		return 1;
	}

	// Setup window
	SDL_Window * window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		SDL_Quit();
		std::cout << "Error while loading window" << std::endl;
		return 1;
	}

	// Setup renderer
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Error while creating renderer : %s", SDL_GetError());
		return EXIT_FAILURE;
	}

    // Setup events
    SDL_Event event;

	// Textures
	SDL_Surface* tileset = SDL_LoadBMP("textures/wall.bmp");
	if (!tileset) // Error
	{
		printf("Loading failed : wall.bmp\n");
		SDL_Quit();
		system("pause");
		exit(-1);
	}
	SDL_Surface* screen = SDL_LoadBMP("textures/bg.bmp");
	if (!screen) // Error
	{
		printf("Loading failed : bg.bmp\n");
		SDL_Quit();
		system("pause");
		exit(-1);
	}

	SDL_Surface* player = SDL_LoadBMP("textures/snake.bmp");
	if (!player) // Error
	{
		printf("Loading failed : snake.bmp\n");
		SDL_Quit();
		system("pause");
		exit(-1);
	}
	SDL_SetColorKey(player,SDL_TRUE,SDL_MapRGB(player->format, 238, 0, 255));

	// Creating texture from surface
	SDL_Texture* texture_wall = SDL_CreateTextureFromSurface(renderer, tileset);
	SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_Texture * snake_texture = SDL_CreateTextureFromSurface(renderer, player);

    SDL_FreeSurface(tileset);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(player);

	SDL_RenderCopy(renderer, background, NULL, NULL);

	// Opening map file
	FILE *fmap;
	fmap = fopen("levels/niv2.txt", "r");

	// Initializing 2D table
	float** table2D;
	table2D = (float**)malloc(map_width * sizeof(float*));

	for (int dim_allocated = 0;dim_allocated < map_width; dim_allocated++) {
		table2D[dim_allocated] = (float*)malloc(map_height * sizeof(float));
	}

	// Attributing 2D table values from map file

	int car_actuel;
	int i, j;

	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			car_actuel = fgetc(fmap);
			if(car_actuel != 10 && car_actuel != -1) {
				table2D[i][j] = car_actuel;
				std::cout << table2D[i][j];
			}
		}
	}

	DrawMap(screen, texture_wall, table2D, map_width, map_height, tile_width, tile_height, renderer);
    SDL_RenderPresent(renderer);

	// Snake spawn position
	Snake snake;
	snake.position.x = 1;
	snake.position.y = 1;

	// Snake head sprite clip
	SDL_Rect snake_head_clip;
    snake_head_clip ={90, 30, 30, 30};
	// Events
	int quit = 0;
    while (!quit)
    {
        SDL_WaitEvent(&event);
        // Variables used for collisions detection
        i = snake.position.x;
        j = snake.position.y;

        // Removing snake textures
        SDL_Rect grass_rect_src = { 0, 0, 30, 30};
        SDL_Rect grass_rect_dest = { snake.position.x*30, snake.position.y*30, 30, 30};

        SDL_RenderCopy(renderer, background, &grass_rect_src, &grass_rect_dest);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: SDL_Quit(); return 0; break;
                      case SDLK_UP:
                        if (table2D[i][--j] == 49) { // Collision
                            break;
                        }
                        else {
                            snake.position.y = --snake.position.y;
                            snake.direction = 1;
                            snake_head_clip = {90, 0, 30, 30};
                            break;
                        };
                    case SDLK_LEFT:
                         if (table2D[--i][j] == 49) {
                            break;
                        }
                        else {
                            snake.position.x = --snake.position.x;
                            snake.direction = 2;
                            snake_head_clip = {90, 90, 30, 30};
                            break;
                        };
                    case SDLK_RIGHT:
                        if (table2D[++i][j] == 49) {
                            break;
                        }
                        else {
                            snake.position.x = ++snake.position.x;
                            snake.direction = 3;
                            snake_head_clip = {90, 30, 30, 30};
                            break;
                        };
                    case SDLK_DOWN:
                        if (table2D[i][++j] == 49) {
                            break;
                        }
                        else {
                            snake.position.y = ++snake.position.y;
                            snake.direction = 4;
                            snake_head_clip = {90, 60, 30, 30};
                            break;
                        };

                }
                break;

        }
        SDL_Rect snake_rect_dest = { snake.position.x*30, snake.position.y*30, 30, 30};
        SDL_RenderCopy(renderer, snake_texture, &snake_head_clip, &snake_rect_dest);
        SDL_RenderPresent(renderer);
    }

    // Free memory
    free(table2D);

}
