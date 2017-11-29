#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

///////////////////////////////////////////////////////
// Structure describing Snake

struct Position {
    int x;
    int y;
};

typedef struct Snake Snake; // Avoid to write "struct Snake" for each new alias of the structure
struct Snake {
    struct Position position; // Position is coordinates from x,y axis
    int direction;
    // Direction where the snake is moving
    // 1 : UP
    // 2 : LEFT
    // 3 : RIGHT
    // 4 : DOWN
    int length; // Length of the snake (without head and tail)
    int score;
};



void snake_clip(int direction);

#endif // SNAKE_H_INCLUDED
