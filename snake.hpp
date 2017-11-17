

///////////////////////////////////////////////////////
// Structure describing Snake

struct Snake {
    struct position; // Position is coordinates from x,y axis
    int direction;
    // Direction where the snake is moving
    // 1 : UP
    // 2 : LEFT
    // 3 : RIGHT
    // 4 : DOWN
    int length; // Length of the snake (without head and tail)
    int score;
};

struct Position {
    int x;
    int y;
};
