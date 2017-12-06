#include <SDL2/SDL.h>
#include "game.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

///////////////////////////////////////////////////////
// Map drawing function
////
void DrawMap(SDL_Surface* screen, SDL_Texture* wall, float** table, int map_width, int map_height, int tile_width, int tile_height, SDL_Renderer * sdlRenderer)
{
	int i, j;
	SDL_Rect Rect_dest;
    SDL_Rect Rect_source = {0, 0, 30, 30};
	Rect_dest.w = tile_width;
	Rect_dest.h = tile_height;
	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			if (table[i][j] == 49) {
				Rect_dest.x = i*tile_width;
				Rect_dest.y = j*tile_height;
				SDL_RenderCopy(sdlRenderer, wall, &Rect_source, &Rect_dest);
			}
		}
	}
}


///////////////////////////////////////////////////////
// Fruit drawing function
void DrawFruits (struct Apple* apple, SDL_Texture* apple_texture,float** table, int map_width, int map_height, int tile_width, int tile_height, SDL_Renderer * sdlRenderer)
{
    int randHeight, randWidth;
    srand(time(NULL));
    randHeight = rand() % map_height;
    randWidth = rand() % map_width;
    SDL_Rect Rect_source = {0, 0, 30, 30};;
	SDL_Rect Rect_dest;
	Rect_dest.w = tile_width;
	Rect_dest.h = tile_height;
	apple->position.y = randHeight;
	apple->position.x = randWidth;
	std::cout<<randHeight;
	std::cout<<" \n ";
    std::cout<< randWidth;
	std::cout<<" \n ";
    std::cout<<table[randWidth][randHeight];
			if (table[randWidth][randHeight] != 49)  {
				Rect_dest.x = randWidth*tile_width;
				Rect_dest.y = randHeight*tile_height;
				SDL_RenderCopy(sdlRenderer, apple_texture, &Rect_source, &Rect_dest);
			}
			else(DrawFruits(apple, apple_texture,table, map_width,map_height,tile_width,tile_height,sdlRenderer));

}

