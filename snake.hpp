#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED
#include "game.hpp"

///////////////////////////////////////////////////////
// Structure describing Snake

struct Position {
    int x;
    int y;
};

struct Direction {
    int current;
    int old;
};

typedef struct Snake Snake; // Avoid to write "struct Snake" for each new alias of the structure
struct Snake {
    struct Position position; // Position is coordinates from x,y axis
    struct Position rotation;
    struct Direction direction;
    // Direction where the snake is moving
    // 0 : PAUSE
    // 1 : UP
    // 2 : LEFT
    // 3 : RIGHT
    // 4 : DOWN
    int length; // Length of the snake (without head and tail)
    int score;
};

void DrawSnake (struct Snake* snake,SDL_Texture* bg,const SDL_Rect snake_head_clip,const SDL_Rect snake_body_clip,const SDL_Rect snake_tail_clip, SDL_Texture* snake_texture, SDL_Renderer* sdlRenderer, int* tailX, int* tailY,int prevX,int prevY,int prev2X,int prev2Y);

typedef struct Apple Apple; // Avoid to write "struct Apple" for each new alias of the structure
struct Apple {
    struct Position position; // Position is coordinates from x,y axis
};

#endif // SNAKE_H_INCLUDED
