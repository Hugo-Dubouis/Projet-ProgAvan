#include <SDL2/SDL.h>
#include "snake.hpp"
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

void DrawMap(SDL_Surface* screen, SDL_Texture* wall, float** table, int map_width, int map_height, int tile_width, int tile_height, SDL_Renderer * sdlRenderer);
void DrawFruits (SDL_Texture* apple,float** table, int map_width, int map_height, int tile_width, int tile_height, SDL_Renderer * sdlRenderer);

typedef struct Apple Apple; // Avoid to write "struct Apple" for each new alias of the structure
struct Apple {
    struct Position position; // Position is coordinates from x,y axis
};

#endif // GAME_H_INCLUDED
