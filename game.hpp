#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL2/SDL.h>
#include "snake.hpp"

void SetupSDL2();
void DrawMap(SDL_Surface* screen, SDL_Texture* wall, float** table, int map_width, int map_height, int tile_width, int tile_height, SDL_Renderer * sdlRenderer);
void DrawFruits (struct Apple* apple, SDL_Texture* apple_texture,float** table, int map_width, int map_height, int tile_width, int tile_height, SDL_Renderer * sdlRenderer, struct Snake* snake, int* tailX, int* tailY);
void Collision (struct Apple* apple, struct Snake* snake, SDL_Texture* apple_texture,float** table, int map_width, int map_height, int tile_width, int tile_height, SDL_Renderer * sdlRenderer,bool* gameOver,int* tailX, int* tailY);

#endif // GAME_H_INCLUDED
