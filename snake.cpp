#include "snake.hpp"
#include <iostream>

void DrawSnake (struct Snake* snake,SDL_Texture* bg,const SDL_Rect snake_head_clip,const SDL_Rect snake_body_clip,const SDL_Rect snake_tail_clip, SDL_Texture* snake_texture, SDL_Renderer* sdlRenderer, int* tailX, int* tailY,int prevX,int prevY,int prev2X,int prev2Y) {

        // UP
        if(snake->direction == 1 && snake->direction != 4) {
            snake->position.y = --snake->position.y;
        }
        // LEFT
        else if (snake->direction == 2 && snake->direction != 3) {
            snake->position.x = --snake->position.x;
        }
        // RIGHT
        else if (snake->direction == 3 && snake->direction != 2) {
            snake->position.x = ++snake->position.x;
        }
        // DOWN
        else if (snake->direction == 4 && snake->direction != 1) {
            snake->position.y = ++snake->position.y;
        }
        else {}

        // Snake tail render
        for(int k = 0; k <= snake->length; k++) {
            // Moving parts of the snake
            prev2X = tailX[k];
            prev2Y= tailY[k];
            tailX[k] = prevX;
            tailY[k] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
            // Coordinates std::cout << "X =" << tailX[k] << " Y =" << tailY[k] << std::endl;
            std::cout << "Length : " << snake->length << std::endl;

            if ((k!=(snake->length)-1)) {
                // Render body
                SDL_Rect snake_body_rect_dest = { tailX[k]*30, tailY[k]*30, 30, 30};
                SDL_RenderCopy(sdlRenderer, snake_texture, &snake_body_clip, &snake_body_rect_dest);
                std::cout << "k : " << k << std::endl;

            }

             // Removing body parts
            SDL_Rect grass_rect_src = { 0, 0, 30, 30};
            SDL_Rect grass_rect_dest1 = { tailX[((snake->length)-1)]*30, tailY[((snake->length)-1)]*30, 30, 30};
            SDL_RenderCopy(sdlRenderer, bg, &grass_rect_src, &grass_rect_dest1);

            // Render tails
            SDL_Rect snake_tail_rect_dest = { tailX[(snake->length)-1]*30, tailY[(snake->length)-1]*30, 30, 30};
            SDL_RenderCopy(sdlRenderer, snake_texture, &snake_tail_clip, &snake_tail_rect_dest);

             // Removing body parts
            SDL_Rect grass_rect_dest2 = { tailX[((snake->length))]*30, tailY[((snake->length))]*30, 30, 30};
            SDL_RenderCopy(sdlRenderer, bg, &grass_rect_src, &grass_rect_dest2);

        }

        // Snake head render
        SDL_Rect snake_rect_dest = { snake->position.x*30, snake->position.y*30, 30, 30};
        SDL_RenderCopy(sdlRenderer, snake_texture, &snake_head_clip, &snake_rect_dest);
}
