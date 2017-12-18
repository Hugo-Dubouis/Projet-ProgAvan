#include "snake.hpp"
#include <iostream>

void DrawSnake (struct Snake* snake,SDL_Texture* bg,const SDL_Rect snake_head_clip,const SDL_Rect snake_body_clip,const SDL_Rect snake_tail_clip, SDL_Texture* snake_texture, SDL_Renderer* sdlRenderer, int* tailX, int* tailY,int prevX,int prevY,int prev2X,int prev2Y) {

        // UP
        if(snake->direction.current == 1 && snake->direction.current != 4) {
            snake->position.y = --snake->position.y;
        }
        // LEFT
        else if (snake->direction.current == 2 && snake->direction.current != 3) {
            snake->position.x = --snake->position.x;
        }
        // RIGHT
        else if (snake->direction.current == 3 && snake->direction.current != 2) {
            snake->position.x = ++snake->position.x;
        }
        // DOWN
        else if (snake->direction.current == 4 && snake->direction.current != 1) {
            snake->position.y = ++snake->position.y;
        }
        else {}

        /////////////////////
        // Snake rendering
        for(int k = 0; k <= snake->length; k++) {
            // Moving parts of the snake
            prev2X = tailX[k];
            prev2Y= tailY[k];
            tailX[k] = prevX;
            tailY[k] = prevY;
            prevX = prev2X;
            prevY = prev2Y;

            SDL_Rect grass_rect_src = { 0, 0, 30, 30};

            // Variable for memorizing "k" where the snake changed direction
            int memK;
            int memDir;
            SDL_Rect snake_tail_clip2;

            // First we check if there are a rotation (the player changed the direction of the snake)
            // If yes : we remove the snake body textures after the rotation and we draw the angle texture
            // If no : we draw the snake body textures before and after the rotation.
            if((tailX[k] == snake->rotation.x) && (tailY[k] == snake->rotation.y)) {
                memK = k;
                memDir = snake->direction.old;
                if ((snake->direction.old == 2 && snake->direction.current == 1) || (snake->direction.old == 4 && snake->direction.current == 3)) {
                    // Removing body parts
                    SDL_Rect grass_rect_dest2 = { tailX[k]*30, tailY[k]*30, 30, 30};
                    SDL_RenderCopy(sdlRenderer, bg, &grass_rect_src, &grass_rect_dest2);
                    // Rendering top to right angle
                    SDL_Rect snake_angle_dest = { tailX[k]*30, tailY[k]*30, 30, 30};
                    SDL_Rect snake_angle_src = { 0, 0, 30, 30};
                    SDL_RenderCopy(sdlRenderer, snake_texture, &snake_angle_src, &snake_angle_dest);

                }
                else if ((snake->direction.old == 3 && snake->direction.current == 1) || (snake->direction.old == 4 && snake->direction.current == 2)) {
                    // Removing body parts
                    SDL_Rect grass_rect_dest2 = { tailX[k]*30, tailY[k]*30, 30, 30};
                    SDL_RenderCopy(sdlRenderer, bg, &grass_rect_src, &grass_rect_dest2);
                    // Rendering top to left angle
                    SDL_Rect snake_angle_dest = { tailX[k]*30, tailY[k]*30, 30, 30};
                    SDL_Rect snake_angle_src = { 30, 0, 30, 30};
                    SDL_RenderCopy(sdlRenderer, snake_texture, &snake_angle_src, &snake_angle_dest);

                }
                else if ((snake->direction.old == 1 && snake->direction.current == 3) || (snake->direction.old == 2 && snake->direction.current == 4)) {
                    // Removing body parts
                    SDL_Rect grass_rect_dest2 = { tailX[k]*30, tailY[k]*30, 30, 30};
                    SDL_RenderCopy(sdlRenderer, bg, &grass_rect_src, &grass_rect_dest2);
                    // Rendering bottom to right angle
                    SDL_Rect snake_angle_dest = { tailX[k]*30, tailY[k]*30, 30, 30};
                    SDL_Rect snake_angle_src = { 0, 30, 30, 30};
                    SDL_RenderCopy(sdlRenderer, snake_texture, &snake_angle_src, &snake_angle_dest);
                }
                else if ((snake->direction.old == 1 && snake->direction.current == 2) || (snake->direction.old == 3 && snake->direction.current == 4)) {
                    // Removing body parts
                    SDL_Rect grass_rect_dest2 = { tailX[k]*30, tailY[k]*30, 30, 30};
                    SDL_RenderCopy(sdlRenderer, bg, &grass_rect_src, &grass_rect_dest2);
                    // Rendering bottom to left angle
                    SDL_Rect snake_angle_dest = { tailX[k]*30, tailY[k]*30, 30, 30};
                    SDL_Rect snake_angle_src = { 30, 30, 30, 30};
                    SDL_RenderCopy(sdlRenderer, snake_texture, &snake_angle_src, &snake_angle_dest);

                }
                else {}
            }
            else {
                if (k < memK)  {
                    // Render body
                    SDL_Rect snake_body_rect_dest = { tailX[k]*30, tailY[k]*30, 30, 30};
                    SDL_RenderCopy(sdlRenderer, snake_texture, &snake_body_clip, &snake_body_rect_dest);
                }
                else {}
            }
            // Removing body parts for snake move
            SDL_Rect grass_rect_dest2 = { tailX[((snake->length))]*30, tailY[((snake->length))]*30, 30, 30};
            SDL_RenderCopy(sdlRenderer, bg, &grass_rect_src, &grass_rect_dest2);
        }

        // Snake head render
        SDL_Rect snake_rect_dest = { snake->position.x*30, snake->position.y*30, 30, 30};
        SDL_RenderCopy(sdlRenderer, snake_texture, &snake_head_clip, &snake_rect_dest);
}
