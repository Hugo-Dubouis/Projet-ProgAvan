#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.hpp"
#include "snake.hpp"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {

   	////////////////////////////////
	// Settings

    // Map width and height
    int map_width = 20;
    int map_height = 21;
    int dim_allocated = 0;

    // Blocs dimensions
    const int tile_width = 30;
    const int tile_height = 30;

	// Snake
	Snake snake;
	snake.position.x = 5;
	snake.position.y = 5;
	snake.direction.current = 0;

 	// Snake head sprite clip
	SDL_Rect snake_head_clip;
    snake_head_clip ={90, 30, 30, 30};

    // Snake body sprite clip
    SDL_Rect snake_body_clip;
    SDL_Rect snake_tail_clip;

    // Snake tail
    int tailX[100], tailY[100];
    snake.length = 2;

    // Apple
    Apple apple;

    // GameOver
    bool gameOver = false;

  	////////////////////////////////
	// Initializing SDL2 & SDL_TTF
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){ std::cout << "Error while initializing SDL" << std::endl; return 1; }

    TTF_Init();

	////////////////////////////////
	// Window
	SDL_Window * window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 630, SDL_WINDOW_SHOWN);

	////////////////////////////////
	// Renderer
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) { printf("Error while creating renderer : %s", SDL_GetError()); return EXIT_FAILURE;}

    ////////////////////////////////
    // Events
    SDL_Event event;

    ////////////////////////////////
	// Surfaces & Textures
	SDL_Surface* tileset = SDL_LoadBMP("textures/wall.bmp");
	if (!tileset) { printf("Loading failed : wall.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }
	SDL_Surface* screen = SDL_LoadBMP("textures/bg.bmp");
	if (!screen) { printf("Loading failed : bg.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* player = SDL_LoadBMP("textures/snake.bmp");
	if (!player) { printf("Loading failed : snake.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_SetColorKey(player,SDL_TRUE,SDL_MapRGB(player->format, 238, 0, 255));

    SDL_Surface* apple_surface = SDL_LoadBMP("textures/apple.bmp");
    if (!apple_surface) { printf("Loading failed : apple.bmp\n"); SDL_Quit(); system("pause"); exit(-1);}

	SDL_SetColorKey(apple_surface,SDL_TRUE,SDL_MapRGB(apple_surface->format, 238, 0, 255));

	SDL_Surface* score = SDL_LoadBMP("textures/score_bg.bmp");
	if (!score) { printf("Loading failed : score_bg.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface * scoreSurface = NULL;

	// Creating texture from surface
	SDL_Texture* texture_wall = SDL_CreateTextureFromSurface(renderer, tileset);
	SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_Texture * snake_texture = SDL_CreateTextureFromSurface(renderer, player);
    SDL_Texture * apple_texture = SDL_CreateTextureFromSurface(renderer, apple_surface);
    SDL_Texture * score_bg = SDL_CreateTextureFromSurface(renderer, score);
    SDL_Texture * scoreTexture = NULL;


    SDL_FreeSurface(tileset);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(player);
    SDL_FreeSurface(apple_surface);
    SDL_FreeSurface(score);

    // Setting the background
	SDL_RenderCopy(renderer, background, NULL, NULL);

	// Setting the score background
	SDL_Rect score_src = { 0, 0, 600, 30};
	SDL_Rect score_dest = { 0, 600, 600, 30};

    ////////////////////////////////
	// Font
    TTF_Font * font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 15);
    if (!font) { printf("Loading failed : arial.ttf\n"); SDL_Quit(); system("pause"); exit(-1); }
    SDL_Color color = { 255, 255, 255 };
    int texW = 0;
    int texH = 0;

    ////////////////////////////////
	// Map file reading

	// Opening map file
	FILE *fmap;
	fmap = fopen("levels/niv2.txt", "r");

	// Initializing 2D table
	float** table2D;
	table2D = (float**)malloc(map_width * sizeof(float*));

	for (dim_allocated; dim_allocated < map_width; dim_allocated++) {
		table2D[dim_allocated] = (float*)malloc(map_height * sizeof(float));
	}

	// Attributing 2D table values from map file

	int car_actuel;

	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			car_actuel = fgetc(fmap);
			if(car_actuel != 10 && car_actuel != -1) {
				table2D[i][j] = car_actuel;
			}
		}
	}

    // 1st fruit drawing
	DrawFruits(&apple,apple_texture,table2D,map_width,map_height,tile_width,tile_height,renderer);

    ////////////////////////////////
	// Main Game loop
    while (!gameOver)
    {
        // Snake head position
        tailX[0] = snake.position.x;
        tailY[0] = snake.position.y;

        // Snake
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;

        // Removing snake textures
        SDL_Rect grass_rect_src = { 0, 0, 30, 30};
        SDL_Rect grass_rect_dest = { snake.position.x*30, snake.position.y*30, 30, 30};
        SDL_RenderCopy(renderer, background, &grass_rect_src, &grass_rect_dest);

        //  Events management
        while (SDL_PollEvent(&event)) {
            switch(event.type)
            {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: SDL_Quit(); return 0; break;
                    case SDLK_UP:
                        if(snake.direction.current != 4 && snake.direction.current != 1) {
                            snake.direction.current = 1;
                            snake.rotation.x = snake.position.x;
                            snake.rotation.y = snake.position.y;
                            std::cout << "X = " << snake.rotation.x << " Y= " << snake.rotation.y << std::endl;
                            snake_head_clip = {90, 0, 30, 30};
                            snake_body_clip ={60, 30, 30, 30};
                            snake_tail_clip ={0, 60, 30, 30};
                            break;
                        }
                        else {
                            break;
                        }

                    case SDLK_LEFT:
                        if(snake.direction.current != 3 && snake.direction.current != 2) {
                            snake.direction.current = 2;
                            snake.rotation.x = snake.position.x;
                            snake.rotation.y = snake.position.y;
                            std::cout << "X = " << snake.rotation.x << " Y= " << snake.rotation.y << std::endl;
                            snake_head_clip = {90, 90, 30, 30};
                            snake_body_clip ={60, 0, 30, 30};
                            snake_tail_clip ={60, 60, 30, 30};
                            break;
                        }
                        else {
                            break;
                        }
                    case SDLK_RIGHT:
                         if(snake.direction.current != 2 && snake.direction.current != 3) {
                            snake.direction.current = 3;
                            snake.rotation.x = snake.position.x;
                            snake.rotation.y = snake.position.y;
                            std::cout << "X = " << snake.rotation.x << " Y= " << snake.rotation.y << std::endl;
                            snake_head_clip = {90, 30, 30, 30};
                            snake_body_clip ={60, 0, 30, 30};
                            snake_tail_clip ={0, 90, 30, 30};
                            break;
                        }
                        else {
                            break;
                        }
                    case SDLK_DOWN:
                          if(snake.direction.current != 1 && snake.direction.current != 4) {
                            snake.direction.current = 4;
                            snake.rotation.x = snake.position.x;
                            snake.rotation.y = snake.position.y;
                            std::cout << "X = " << snake.rotation.x << " Y= " << snake.rotation.y << std::endl;
                            snake_head_clip = {90, 60, 30, 30};
                            snake_body_clip ={60, 30, 30, 30};
                            snake_tail_clip ={30, 60, 30, 30};
                            break;
                        }
                        else {
                            break;
                        }
                }
                break;
            }
        }

         // Map drawing
        DrawMap(screen,texture_wall, table2D, map_width, map_height, tile_width, tile_height, renderer);
        // Snake drawing
        DrawSnake(&snake,background,snake_head_clip,snake_body_clip,snake_tail_clip,snake_texture,renderer,tailX,tailY,prevX,prevY,prev2X,prev2Y);
        // Collision detection
        Collision(&apple,&snake,apple_texture,table2D,map_width,map_height,tile_width,tile_height,renderer,&gameOver,tailX,tailY);
        // Memorizing old snake direction
        snake.direction.old = snake.direction.current;
        // Score background
        SDL_RenderCopy(renderer, score_bg, &score_src, &score_dest);

        // Score
        char score[10] = "";
        sprintf(score, "Score : %d", (snake.length-2)*100);

        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);

        SDL_Surface * scoreSurface = TTF_RenderText_Blended(font, score, color);
        SDL_Texture * scoreTexture = SDL_CreateTextureFromSurface(renderer,scoreSurface);

        SDL_QueryTexture(scoreTexture, NULL, NULL, &texW, &texH);
        SDL_Rect score_dest = { 5, 605, texW, texH};

        SDL_RenderCopy(renderer, scoreTexture, NULL, &score_dest);

        SDL_RenderPresent(renderer);

        SDL_Delay(110);
    }
    // Free memory
    free(table2D);

	SDL_DestroyWindow(window);

	TTF_Quit();
	SDL_Quit();

	return 0;


}
