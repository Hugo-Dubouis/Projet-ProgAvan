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

    // MainMenu
    bool mainMenu = false;

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
	// Game
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

	// Menu
	SDL_Surface* screenMenu = SDL_LoadBMP("textures/menu_bg.bmp");
	if (!screenMenu) { printf("Loading failed : menu_bg.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu1 = SDL_LoadBMP("textures/mainMenu1.bmp");
	if (!buttonMenu1) { printf("Loading failed : mainMenu1.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu1_selected = SDL_LoadBMP("textures/mainMenu1_selected.bmp");
	if (!buttonMenu1_selected) { printf("Loading failed : mainMenu1_selected.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu2 = SDL_LoadBMP("textures/mainMenu2.bmp");
	if (!buttonMenu2) { printf("Loading failed : mainMenu2.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu2_selected = SDL_LoadBMP("textures/mainMenu2_selected.bmp");
	if (!buttonMenu2_selected) { printf("Loading failed : mainMenu2_selected.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu3 = SDL_LoadBMP("textures/mainMenu3.bmp");
	if (!buttonMenu3) { printf("Loading failed : mainMenu3.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu3_selected = SDL_LoadBMP("textures/mainMenu3_selected.bmp");
	if (!buttonMenu3_selected) { printf("Loading failed : mainMenu3_selected.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu4 = SDL_LoadBMP("textures/mainMenu4.bmp");
	if (!buttonMenu4) { printf("Loading failed : mainMenu4.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu4_selected = SDL_LoadBMP("textures/mainMenu4_selected.bmp");
	if (!buttonMenu4_selected) { printf("Loading failed : mainMenu4_selected.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	// Creating texture from surface
	// Game
	SDL_Texture* texture_wall = SDL_CreateTextureFromSurface(renderer, tileset);
	SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_Texture * snake_texture = SDL_CreateTextureFromSurface(renderer, player);
    SDL_Texture * apple_texture = SDL_CreateTextureFromSurface(renderer, apple_surface);
    SDL_Texture * score_bg = SDL_CreateTextureFromSurface(renderer, score);
    SDL_Texture * scoreTexture = NULL;
    // Menu
    SDL_Texture * menu_bg = SDL_CreateTextureFromSurface(renderer, screenMenu);
    SDL_Texture * menuPlay = SDL_CreateTextureFromSurface(renderer, buttonMenu1);
    SDL_Texture * menuPlay_selected = SDL_CreateTextureFromSurface(renderer, buttonMenu1_selected);
    SDL_Texture * menuSelectLevel = SDL_CreateTextureFromSurface(renderer, buttonMenu2);
    SDL_Texture * menuSelectLevel_selected = SDL_CreateTextureFromSurface(renderer, buttonMenu2_selected);
    SDL_Texture * menuOptions = SDL_CreateTextureFromSurface(renderer, buttonMenu3);
    SDL_Texture * menuOptions_selected = SDL_CreateTextureFromSurface(renderer, buttonMenu3_selected);
    SDL_Texture * menuQuit = SDL_CreateTextureFromSurface(renderer, buttonMenu4);
    SDL_Texture * menuQuit_selected = SDL_CreateTextureFromSurface(renderer, buttonMenu4_selected);

    // Liberating memory used for surface
    SDL_FreeSurface(tileset);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(player);
    SDL_FreeSurface(apple_surface);
    SDL_FreeSurface(score);

	/* Liberating memory used for textures
    SDL_DestroyTexture(texture_wall);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(snake_texture);
    SDL_DestroyTexture(apple_texture);
    SDL_DestroyTexture(score_bg);
    SDL_DestroyTexture(scoreTexture);
    */

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
    // Main menu
    int selectedMenu =0;
    // Setting Button coordinates
    SDL_Rect buttonMenu1_dst = { 114, 216, 372, 56};
    SDL_Rect buttonMenu2_dst = { 114, 300, 372, 56};
    SDL_Rect buttonMenu3_dst = { 114, 384, 372, 56};
    SDL_Rect buttonMenu4_dst = { 114, 468, 372, 56};
	while(!mainMenu) {


        // Main menu background
        SDL_RenderCopy(renderer, menu_bg, NULL, NULL);

        SDL_WaitEvent(&event);
            switch(event.type)
            {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: SDL_Quit(); return 0; break;
                    case SDLK_UP :
                        if(selectedMenu>0) {
                           selectedMenu--;
                        }
                        else {selectedMenu=3;}
                        break;
                    case SDLK_DOWN :
                        if(selectedMenu < 3) {
                            selectedMenu++;
                        }
                        else {selectedMenu=0;}
                        break;
                    case SDLK_RETURN :
                        if(selectedMenu == 0) {
                            mainMenu = true;


                        }
                        else if (selectedMenu == 1) {
                            std::cout << "Work in progress !" << std::endl;
                        }
                        else if (selectedMenu == 2) {
                            std::cout << "Work in progress !" << std::endl;
                        }
                        else if (selectedMenu == 3){
                            SDL_Quit(); return 0; break;
                        }
                        else {}
                }
            }
        SDL_RenderCopy(renderer, menuPlay, NULL, &buttonMenu1_dst);
        SDL_RenderCopy(renderer, menuSelectLevel, NULL, &buttonMenu2_dst);
        SDL_RenderCopy(renderer, menuOptions, NULL, &buttonMenu3_dst);
        SDL_RenderCopy(renderer, menuQuit, NULL, &buttonMenu4_dst);

        // Play button selected
        if(selectedMenu == 0) {
            SDL_RenderCopy(renderer, menuPlay_selected, NULL, &buttonMenu1_dst);
        }
        // SelectLevel button selected
        else if (selectedMenu == 1) {
            SDL_RenderCopy(renderer, menuSelectLevel_selected, NULL, &buttonMenu2_dst);
        }
        // Options button selected
        else if (selectedMenu == 2) {
            SDL_RenderCopy(renderer, menuOptions_selected, NULL, &buttonMenu3_dst);
        }
        // Quit button selected
        else if (selectedMenu == 3){
            SDL_RenderCopy(renderer, menuQuit_selected, NULL, &buttonMenu4_dst);
        }
        else {}

        std::cout << selectedMenu << std::endl;
        SDL_RenderPresent(renderer);
	}

    // Clearing renderer from main menu
    SDL_RenderClear(renderer);

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

    // Setting the background
	SDL_RenderCopy(renderer, background, NULL, NULL);

    // 1st fruit drawing
	DrawFruits(&apple,apple_texture,table2D,map_width,map_height,tile_width,tile_height,renderer,&snake,tailX,tailY);

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
