#include <SDL2/SDL.h>
#include "game.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

///////////////////////////////////////////////////////
// Function setting up the Game
////
void SetupGame()
{

}

///////////////////////////////////////////////////////
// Function setting up SDL2
////
void SetupSDL2()
{

}

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
    srand(time(0));
    randHeight = rand() % ((map_height-1) - 1 + 1);
    randWidth = rand() % ((map_width-1) - 1 + 1);
    SDL_Rect Rect_source = {0, 0, 30, 30};;
	SDL_Rect Rect_dest;
	Rect_dest.w = tile_width;
	Rect_dest.h = tile_height;
	apple->position.x = randWidth;
	apple->position.y = randHeight;
	std::cout<< randWidth << std::endl;
	std::cout<< randHeight << std::endl;
	std::cout<< table[randWidth][randHeight] << std::endl;
			if (table[randWidth][randHeight] != 49)  {
				Rect_dest.x = randWidth*tile_width;
				Rect_dest.y = randHeight*tile_height;
				SDL_RenderCopy(sdlRenderer, apple_texture, &Rect_source, &Rect_dest);
			}
			else(DrawFruits(apple, apple_texture,table, map_width,map_height,tile_width,tile_height,sdlRenderer));

}

///////////////////////////////////////////////////////
// Collision detection function
void Collision (struct Apple* apple, struct Snake* snake,SDL_Texture* apple_texture,float** table, int map_width, int map_height, int tile_width, int tile_height, SDL_Renderer * sdlRenderer, bool* gameOver,int* tailX, int* tailY) {
        // Regenerate Apple if snake position equals apple position
        if  ((snake->position.x == apple->position.x) && (snake->position.y == apple->position.y)) {
            std::cout<< "Fruit!" << std::endl;
            DrawFruits(apple, apple_texture, table, map_width, map_height,tile_width,tile_height,sdlRenderer);
            snake->length++;
        }
        // Check if the snake touch his tail
        if(snake->length != 2) {
            for(int i = 0; i < snake->length; i++) {
                if((snake->position.x == tailX[i]) && (snake->position.y == tailY[i])) {
                    *gameOver = true;
                }
            }
        }

        /*
        std::cout<< "SNAKE X : " << snake->position.x << " | SNAKE Y : " << snake->position.y  << std::endl;
        std::cout<< "APPLE X : " << apple->position.x << " | APPLE Y : " << apple->position.y  << std::endl;
        std::cout<< table[snake->position.x][snake->position.y] << std::endl;
        */

        // Detect if the snake touch the walls
        if (table[snake->position.x][snake->position.y] == 49) {
            *gameOver = true;
        }
}

