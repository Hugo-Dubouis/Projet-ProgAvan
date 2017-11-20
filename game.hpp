#include <SDL2/SDL.h>
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

void DrawMap(SDL_Surface* screen, SDL_Texture* wall, float** table, int map_width, int map_height, int tile_width, int tile_height, SDL_Renderer * sdlRenderer);

#endif // GAME_H_INCLUDED
