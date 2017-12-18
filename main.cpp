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
	snake.score = 0;
	snake.speed = 110;

 	// Snake head sprite clip
	SDL_Rect snake_head_clip ={90, 30, 30, 30};

    // Snake body sprite clip
    SDL_Rect snake_body_clip;
    SDL_Rect snake_tail_clip;

    // Snake tail [Max size is 100 body parts]
    int tailX[100], tailY[100];
    snake.length = 2;

    // Apple
    Apple apple;

    // Menu booleans
    bool mainMenu = false;
    bool mainMenu_SelectLevel = false;
    bool mainMenu_Options = false;

    // gameRunning boolean
    bool gameRunning = false;

    // gameOver boolean
    bool gameOver = false;

    // Game Mode
    int gameMode = 0;
    // 0 : Any game mode selected.
    // 1 : Normal mode : You progress from level 1 to level 6.
    // When you reach a score of 2500 on a level you pass to the next level.
    // 2 : Score Mode : You choose a level and you can try to get the best score.

    // Game commands
    int gameCommands = 0;
    // 0 : Normal mode with keyboard arrows
    // 1 : ZQSD mode on the keyboard
    // 2 : Joystick mode

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

	SDL_Surface* score_bg_surface = SDL_LoadBMP("textures/score_bg.bmp");
	if (!score_bg_surface) { printf("Loading failed : score_bg.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface * scoreSurface = NULL;

	// Menu
	SDL_Surface* screenMenu = SDL_LoadBMP("textures/menu_bg.bmp");
	if (!screenMenu) { printf("Loading failed : menu_bg.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* screenMenuSelectLevel = SDL_LoadBMP("textures/menu_SelectLevel_bg.bmp");
	if (!screenMenuSelectLevel) { printf("Loading failed : menu_SelectLevel_bg.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* screenMenuSettings = SDL_LoadBMP("textures/bg_settings.bmp");
	if (!screenMenuSettings) { printf("Loading failed : bg_settings.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu = SDL_LoadBMP("textures/buttonMenu.bmp");
	if (!buttonMenu) { printf("Loading failed : buttonMenu.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* buttonMenu_selected = SDL_LoadBMP("textures/buttonMenu_selected.bmp");
	if (!buttonMenu_selected) { printf("Loading failed : buttonMenu_selected.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_Surface* arrowMenuUp_surface = SDL_LoadBMP("textures/arrowUp.bmp");
	if (!arrowMenuUp_surface) { printf("Loading failed : arrowUp.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_SetColorKey(arrowMenuUp_surface,SDL_TRUE,SDL_MapRGB(arrowMenuUp_surface->format, 238, 0, 255));

	SDL_Surface* arrowMenuDown_surface = SDL_LoadBMP("textures/arrowDown.bmp");
	if (!arrowMenuDown_surface) { printf("Loading failed : arrowDown.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_SetColorKey(arrowMenuDown_surface,SDL_TRUE,SDL_MapRGB(arrowMenuDown_surface->format, 238, 0, 255));

	SDL_Surface* arrowMenuLeft_surface = SDL_LoadBMP("textures/arrow_left.bmp");
	if (!arrowMenuLeft_surface) { printf("Loading failed : arrow_left.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_SetColorKey(arrowMenuLeft_surface,SDL_TRUE,SDL_MapRGB(arrowMenuLeft_surface->format, 238, 0, 255));

	SDL_Surface* arrowMenuRight_surface = SDL_LoadBMP("textures/arrow_right.bmp");
	if (!arrowMenuRight_surface) { printf("Loading failed : arrow_right.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	SDL_SetColorKey(arrowMenuRight_surface,SDL_TRUE,SDL_MapRGB(arrowMenuRight_surface->format, 238, 0, 255));

	// GameOver Menu
    SDL_Surface* screenGameOver = SDL_LoadBMP("textures/bg_gameOver.bmp");
	if (!screenGameOver) { printf("Loading failed : bg_gameOver.bmp\n"); SDL_Quit(); system("pause"); exit(-1); }

	// Creating texture from surface
	// Game
	SDL_Texture* texture_wall = SDL_CreateTextureFromSurface(renderer, tileset);
	SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_Texture * snake_texture = SDL_CreateTextureFromSurface(renderer, player);
    SDL_Texture * apple_texture = SDL_CreateTextureFromSurface(renderer, apple_surface);
    SDL_Texture * score_bg = SDL_CreateTextureFromSurface(renderer, score_bg_surface);
    SDL_Texture * scoreTexture = NULL;
    // Menu
    SDL_Texture * menu_bg = SDL_CreateTextureFromSurface(renderer, screenMenu);
    SDL_Texture * menu_SelectLevel_bg = SDL_CreateTextureFromSurface(renderer, screenMenuSelectLevel);
    SDL_Texture * menu_Settings_bg = SDL_CreateTextureFromSurface(renderer, screenMenuSettings);
    SDL_Texture * menuButton = SDL_CreateTextureFromSurface(renderer, buttonMenu);
    SDL_Texture * menuButton_selected = SDL_CreateTextureFromSurface(renderer, buttonMenu_selected);
    SDL_Texture * arrowUp = SDL_CreateTextureFromSurface(renderer, arrowMenuUp_surface);
    SDL_Texture * arrowDown = SDL_CreateTextureFromSurface(renderer, arrowMenuDown_surface);
    SDL_Texture * arrowLeft = SDL_CreateTextureFromSurface(renderer, arrowMenuLeft_surface);
    SDL_Texture * arrowRight = SDL_CreateTextureFromSurface(renderer, arrowMenuRight_surface);

    // Game Over Menu
    SDL_Texture * gameOver_bg = SDL_CreateTextureFromSurface(renderer, screenGameOver);

    // Liberating memory used for surface
    SDL_FreeSurface(tileset);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(player);
    SDL_FreeSurface(apple_surface);
    SDL_FreeSurface(score_bg_surface);
    SDL_FreeSurface(screenMenu);
    SDL_FreeSurface(screenMenuSelectLevel);
    SDL_FreeSurface(screenMenuSettings);
    SDL_FreeSurface(buttonMenu);
    SDL_FreeSurface(buttonMenu_selected);
    SDL_FreeSurface(screenGameOver);
    SDL_FreeSurface(arrowMenuUp_surface);
    SDL_FreeSurface(arrowMenuDown_surface);
    SDL_FreeSurface(arrowMenuLeft_surface);
    SDL_FreeSurface(arrowMenuRight_surface);

	// Setting the score background coordinates
	SDL_Rect score_src = { 0, 0, 600, 30};
	SDL_Rect score_dest = { 0, 600, 600, 30};

    ////////////////////////////////
	// Font
    TTF_Font * font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 15);
    if (!font) { printf("Loading failed : OpenSans-Regular.ttf\n"); SDL_Quit(); system("pause"); exit(-1); }
    TTF_Font * fontMenu = TTF_OpenFont("fonts/OpenSans-Bold.ttf", 25);
    if (!fontMenu) { printf("Loading failed : OpenSans-Bold.ttf\n"); SDL_Quit(); system("pause"); exit(-1); }
    SDL_Color color = { 255, 255, 255 };
    int texW = 0;
    int texH = 0;

    ////////////////////////////////
    // Main menu

    // Variables for menu and level selection
    int selectedMenu = 0;
    int selectedLevelMenu = 0;
    int selectedSettingsMenu = 0;
    int selectedSettingsMenu_difficulty = 1;
    int selectedSettingsMenu_commands = 0;
    int selectedGameOverMenu = 0;
    int SelectedLevel;

    // Button coordinates
    SDL_Rect buttonMenu1_dst = { 114, 216, 372, 56};
    SDL_Rect buttonMenu2_dst = { 114, 300, 372, 56};
    SDL_Rect buttonMenu3_dst = { 114, 384, 372, 56};
    SDL_Rect buttonMenu4_dst = { 114, 468, 372, 56};
    SDL_Rect arrowMenuUp_dst = { 282, 240, 37, 37};
    SDL_Rect arrowMenuDown_dst = { 282, 558, 37, 37};
    SDL_Rect arrowMenuLeft1_dst = { 141, 311, 37, 37};
    SDL_Rect arrowMenuRight1_dst = { 422, 311, 37, 37};
    SDL_Rect arrowMenuLeft2_dst = { 141, 395, 37, 37};
    SDL_Rect arrowMenuRight2_dst = { 422, 395, 37, 37};
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
                        if(gameCommands == 0) {
                            if(selectedMenu>0) {
                               selectedMenu--;
                            }
                            else {
                                selectedMenu=3;
                            }
                        }
                        break;
                    case SDLK_DOWN :
                        if(gameCommands == 0) {
                            if(selectedMenu < 3) {
                                selectedMenu++;
                            }
                            else {
                                selectedMenu=0;
                            }
                        }
                        break;
                    case SDLK_z :
                        if(gameCommands == 1) {
                            if(selectedMenu>0) {
                               selectedMenu--;
                            }
                            else {
                                selectedMenu=3;
                            }
                        }
                        break;
                    case SDLK_s :
                        if(gameCommands == 1) {
                            if(selectedMenu < 3) {
                                selectedMenu++;
                            }
                            else {
                                selectedMenu=0;
                            }
                        }
                        break;
                    case SDLK_RETURN :
                        if(selectedMenu == 0) {
                            mainMenu = true;
                            SelectedLevel = 1;
                            gameMode = 1;
                        }
                        else if (selectedMenu == 1) {
                            mainMenu_SelectLevel = true;
                        }
                        else if (selectedMenu == 2) {
                            mainMenu_Options = true;
                        }
                        else if (selectedMenu == 3){
                            SDL_Quit(); return 0; break;
                        }
                        else {}
                     case SDLK_KP_ENTER :
                         if(selectedMenu == 0) {
                            mainMenu = true;
                            SelectedLevel = 1;
                            gameMode = 1;
                        }
                        else if (selectedMenu == 1) {
                            mainMenu_SelectLevel = true;
                        }
                        else if (selectedMenu == 2) {
                            mainMenu_Options = true;
                        }
                        else if (selectedMenu == 3){
                            SDL_Quit(); return 0; break;
                        }

                }
            }
        // Button background
        SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu1_dst);
        SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu2_dst);
        SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu3_dst);
        SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu4_dst);

        // Play button selected
        if(selectedMenu == 0) {
            SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu1_dst);
        }
        // SelectLevel button selected
        else if (selectedMenu == 1) {
            SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu2_dst);
        }
        // Options button selected
        else if (selectedMenu == 2) {
            SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu3_dst);
        }
        // Quit button selected
        else if (selectedMenu == 3){
            SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu4_dst);
        }
        else {}

        // Button Play text
        SDL_Surface * textMenuPlay_surface = TTF_RenderText_Blended(fontMenu, "Jouer", color);
        SDL_Texture * textMenuPlay_texture = SDL_CreateTextureFromSurface(renderer,textMenuPlay_surface);
        SDL_QueryTexture(textMenuPlay_texture, NULL, NULL, &texW, &texH);
        SDL_Rect textMenuPlay_dest = { 265, 225, texW, texH};
        SDL_RenderCopy(renderer, textMenuPlay_texture, NULL, &textMenuPlay_dest);

        // Button SelectLevel text
        SDL_Surface * textMenuSelect_surface = TTF_RenderText_Blended(fontMenu, "Choisir un niveau", color);
        SDL_Texture * textMenuSelect_texture = SDL_CreateTextureFromSurface(renderer,textMenuSelect_surface);
        SDL_QueryTexture(textMenuSelect_texture, NULL, NULL, &texW, &texH);
        SDL_Rect textMenuSelect_dest = { 195, 309, texW, texH};
        SDL_RenderCopy(renderer, textMenuSelect_texture, NULL, &textMenuSelect_dest);

        // Button Settings text
        SDL_Surface * textMenuSettings_surface = TTF_RenderText_Blended(fontMenu, "Options", color);
        SDL_Texture * textMenuSettings_texture = SDL_CreateTextureFromSurface(renderer,textMenuSettings_surface);
        SDL_QueryTexture(textMenuSettings_texture, NULL, NULL, &texW, &texH);
        SDL_Rect textMenuSettings_dest = { 250, 393, texW, texH};
        SDL_RenderCopy(renderer, textMenuSettings_texture, NULL, &textMenuSettings_dest);

        // Button Quit text
        SDL_Surface * textMenuQuit_surface = TTF_RenderText_Blended(fontMenu, "Quitter", color);
        SDL_Texture * textMenuQuit_texture = SDL_CreateTextureFromSurface(renderer,textMenuQuit_surface);
        SDL_QueryTexture(textMenuQuit_texture, NULL, NULL, &texW, &texH);
        SDL_Rect textMenuQuit_dest = { 250, 477, texW, texH};
        SDL_RenderCopy(renderer, textMenuQuit_texture, NULL, &textMenuQuit_dest);

        SDL_RenderPresent(renderer);


        ////////////////////////////////
        // Level selecting menu
        while(mainMenu_SelectLevel) {
            // Clearing renderer from main menu
            SDL_RenderClear(renderer);

            // Main menu background
            SDL_RenderCopy(renderer, menu_SelectLevel_bg, NULL, NULL);

            SDL_WaitEvent(&event);
            switch(event.type)
            {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: SDL_Quit(); return 0; break;
                    case SDLK_UP :
                        if(gameCommands == 0) {
                            if(selectedLevelMenu>0) {
                               selectedLevelMenu--;
                            }
                            else {
                                selectedLevelMenu=5;
                            }
                        }
                        break;
                    case SDLK_DOWN :
                        if(gameCommands == 0) {
                            if(selectedLevelMenu < 5) {
                                selectedLevelMenu++;
                            }
                            else {
                                selectedLevelMenu=0;
                            }
                        }
                        break;
                    case SDLK_z :
                        if(gameCommands == 1) {
                            if(selectedLevelMenu>0) {
                               selectedLevelMenu--;
                            }
                            else {
                                selectedLevelMenu=5;
                            }
                        }
                        break;
                    case SDLK_s :
                        if(gameCommands == 1) {
                            if(selectedLevelMenu < 5) {
                                selectedLevelMenu++;
                            }
                            else {
                                selectedLevelMenu=0;
                            }
                        }
                        break;
                    case SDLK_RETURN :
                        if(selectedLevelMenu == 0) {
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 1;
                            gameMode = 2;
                        }
                        else if (selectedLevelMenu == 1) {
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 2;
                            gameMode = 2;

                        }
                        else if (selectedLevelMenu == 2) {
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 3;
                            gameMode = 2;
                        }
                        else if (selectedLevelMenu == 3){
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 4;
                            gameMode = 2;
                        }
                        else if (selectedLevelMenu == 4){
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 5;
                            gameMode = 2;
                        }
                        else if (selectedLevelMenu == 5){
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 6;
                            gameMode = 2;
                        }
                        else {}
                     case SDLK_KP_ENTER :
                        if(selectedLevelMenu == 0) {
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 1;
                            gameMode = 2;
                        }
                        else if (selectedLevelMenu == 1) {
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 2;
                            gameMode = 2;

                        }
                        else if (selectedLevelMenu == 2) {
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 3;
                            gameMode = 2;
                        }
                        else if (selectedLevelMenu == 3){
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 4;
                            gameMode = 2;
                        }
                        else if (selectedLevelMenu == 4){
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 5;
                            gameMode = 2;
                        }
                        else if (selectedLevelMenu == 5){
                            mainMenu = true;
                            mainMenu_SelectLevel = false;
                            SelectedLevel = 6;
                            gameMode = 2;
                        }
                        else {}

                }
            }

             // Button background
            SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu2_dst);
            SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu3_dst);
            SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu4_dst);

            if(selectedLevelMenu < 3) {
                if(selectedLevelMenu == 0) {
                        SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu2_dst);
                }
                else if (selectedLevelMenu == 1) {
                        SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu3_dst);
                }
                else if (selectedLevelMenu == 2) {
                        SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu4_dst);
                }
                else {}

                // Arrow Down
                SDL_RenderCopy(renderer, arrowDown, NULL, &arrowMenuDown_dst);

                // Button Level 1 text
                SDL_Surface * textMenuSelect_surface = TTF_RenderText_Blended(fontMenu, "Niveau 1", color);
                SDL_Texture * textMenuSelect_texture = SDL_CreateTextureFromSurface(renderer,textMenuSelect_surface);
                SDL_QueryTexture(textMenuSelect_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuSelect_dest = { 245, 309, texW, texH};
                SDL_RenderCopy(renderer, textMenuSelect_texture, NULL, &textMenuSelect_dest);

                // Button Level 2 text
                SDL_Surface * textMenuSettings_surface = TTF_RenderText_Blended(fontMenu, "Niveau 2", color);
                SDL_Texture * textMenuSettings_texture = SDL_CreateTextureFromSurface(renderer,textMenuSettings_surface);
                SDL_QueryTexture(textMenuSettings_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuSettings_dest = { 245, 393, texW, texH};
                SDL_RenderCopy(renderer, textMenuSettings_texture, NULL, &textMenuSettings_dest);

                // Button Level 3 text
                SDL_Surface * textMenuQuit_surface = TTF_RenderText_Blended(fontMenu, "Niveau 3", color);
                SDL_Texture * textMenuQuit_texture = SDL_CreateTextureFromSurface(renderer,textMenuQuit_surface);
                SDL_QueryTexture(textMenuQuit_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuQuit_dest = { 245, 477, texW, texH};
                SDL_RenderCopy(renderer, textMenuQuit_texture, NULL, &textMenuQuit_dest);
            }
            else {
                if(selectedLevelMenu == 3) {
                        SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu2_dst);
                }
                else if (selectedLevelMenu == 4) {
                        SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu3_dst);
                }
                else if (selectedLevelMenu == 5) {
                        SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu4_dst);
                }
                else {}

                // Arrow Up
                SDL_RenderCopy(renderer, arrowUp, NULL, &arrowMenuUp_dst);

                // Button Level 4 text
                SDL_Surface * textMenuSelect_surface = TTF_RenderText_Blended(fontMenu, "Niveau 4", color);
                SDL_Texture * textMenuSelect_texture = SDL_CreateTextureFromSurface(renderer,textMenuSelect_surface);
                SDL_QueryTexture(textMenuSelect_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuSelect_dest = { 245, 309, texW, texH};
                SDL_RenderCopy(renderer, textMenuSelect_texture, NULL, &textMenuSelect_dest);

                // Button Level 5 text
                SDL_Surface * textMenuSettings_surface = TTF_RenderText_Blended(fontMenu, "Niveau 5", color);
                SDL_Texture * textMenuSettings_texture = SDL_CreateTextureFromSurface(renderer,textMenuSettings_surface);
                SDL_QueryTexture(textMenuSettings_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuSettings_dest = { 245, 393, texW, texH};
                SDL_RenderCopy(renderer, textMenuSettings_texture, NULL, &textMenuSettings_dest);

                // Button Level 6 text
                SDL_Surface * textMenuQuit_surface = TTF_RenderText_Blended(fontMenu, "Niveau 6", color);
                SDL_Texture * textMenuQuit_texture = SDL_CreateTextureFromSurface(renderer,textMenuQuit_surface);
                SDL_QueryTexture(textMenuQuit_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuQuit_dest = { 245, 477, texW, texH};
                SDL_RenderCopy(renderer, textMenuQuit_texture, NULL, &textMenuQuit_dest);
            }

            SDL_RenderPresent(renderer);

        }

        ////////////////////////////////
        // Settings menu
        while(mainMenu_Options) {
            // Clearing renderer from main menu
            SDL_RenderClear(renderer);

            // Main menu background
            SDL_RenderCopy(renderer, menu_Settings_bg, NULL, NULL);

            SDL_WaitEvent(&event);
            switch(event.type)
            {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: SDL_Quit(); return 0; break;
                    case SDLK_UP :
                        if(gameCommands == 0) {
                            if(selectedSettingsMenu>0) {
                               selectedSettingsMenu--;
                            }
                            else {
                                selectedSettingsMenu=2;
                            }
                        }
                        break;
                    case SDLK_DOWN :
                        if(gameCommands == 0) {
                            if(selectedSettingsMenu < 2) {
                                selectedSettingsMenu++;
                            }
                            else {
                                selectedSettingsMenu=0;
                            }
                        }
                        break;
                    case SDLK_z :
                        if(gameCommands == 1) {
                            if(selectedSettingsMenu>0) {
                               selectedSettingsMenu--;
                            }
                            else {
                                selectedSettingsMenu=2;
                            }
                        }
                        break;
                    case SDLK_s :
                        if(gameCommands == 1) {
                            if(selectedSettingsMenu < 2) {
                                selectedSettingsMenu++;
                            }
                            else {
                                selectedSettingsMenu=0;
                            }
                        }
                        break;
                    case SDLK_LEFT:
                        if(selectedSettingsMenu == 0) {
                            if(selectedSettingsMenu_difficulty > 0) {
                                selectedSettingsMenu_difficulty--;
                            }
                            else {selectedSettingsMenu_difficulty = 2;}
                        }
                        if(selectedSettingsMenu == 1) {
                            if(selectedSettingsMenu_commands > 0) {
                                selectedSettingsMenu_commands--;
                            }
                            else {selectedSettingsMenu_commands = 1;}
                        }
                        break;
                    case SDLK_RIGHT:
                        if(selectedSettingsMenu == 0) {
                            if(selectedSettingsMenu_difficulty < 2) {
                                selectedSettingsMenu_difficulty++;
                            }
                            else {selectedSettingsMenu_difficulty=0;}
                        }
                        if(selectedSettingsMenu == 1) {
                            if(selectedSettingsMenu_commands < 1) {
                                selectedSettingsMenu_commands++;
                            }
                            else {selectedSettingsMenu_commands = 0;}
                        }
                        break;
                    case SDLK_RETURN :
                        if(selectedSettingsMenu == 2) {
                            mainMenu_Options = false;
                        }
                        else {}
                     case SDLK_KP_ENTER :
                        if(selectedSettingsMenu == 2) {
                            mainMenu_Options = false;
                        }
                        else {}

                }
            }
            // Button background
            SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu2_dst);
            SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu3_dst);
            SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu4_dst);

            if(selectedSettingsMenu == 0) {
                SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu2_dst);
            }
            else if (selectedSettingsMenu == 1) {
                SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu3_dst);
            }
            else if (selectedSettingsMenu == 2) {
                SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu4_dst);
            }
            else {}

            if(selectedSettingsMenu_difficulty == 0) {
                 // Button Easy text
                SDL_Surface * textMenuSelect_surface = TTF_RenderText_Blended(fontMenu, "Facile", color);
                SDL_Texture * textMenuSelect_texture = SDL_CreateTextureFromSurface(renderer,textMenuSelect_surface);
                SDL_QueryTexture(textMenuSelect_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuSelect_dest = { 255, 309, texW, texH};
                SDL_RenderCopy(renderer, textMenuSelect_texture, NULL, &textMenuSelect_dest);
                // Set difficult level (speed)
                snake.speed = 135;
            }
            if (selectedSettingsMenu_difficulty == 1) {
                 // Button Normal text
                SDL_Surface * textMenuSelect_surface = TTF_RenderText_Blended(fontMenu, "Normal", color);
                SDL_Texture * textMenuSelect_texture = SDL_CreateTextureFromSurface(renderer,textMenuSelect_surface);
                SDL_QueryTexture(textMenuSelect_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuSelect_dest = { 255, 309, texW, texH};
                SDL_RenderCopy(renderer, textMenuSelect_texture, NULL, &textMenuSelect_dest);
                // Set difficult level (speed)
                snake.speed = 100;
            }
            if (selectedSettingsMenu_difficulty == 2) {
                 // Button Hard text
                SDL_Surface * textMenuSelect_surface = TTF_RenderText_Blended(fontMenu, "Difficile", color);
                SDL_Texture * textMenuSelect_texture = SDL_CreateTextureFromSurface(renderer,textMenuSelect_surface);
                SDL_QueryTexture(textMenuSelect_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuSelect_dest = { 255, 309, texW, texH};
                SDL_RenderCopy(renderer, textMenuSelect_texture, NULL, &textMenuSelect_dest);
                // Set difficult level (speed)
                snake.speed = 75;
            }

            if(selectedSettingsMenu_commands == 0) {
                 // Button Arrow keyboard text
                SDL_Surface * textMenuSelect_surface = TTF_RenderText_Blended(fontMenu, "Fl�ches (Clavier)", color);
                SDL_Texture * textMenuSelect_texture = SDL_CreateTextureFromSurface(renderer,textMenuSelect_surface);
                SDL_QueryTexture(textMenuSelect_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuSelect_dest = { 200, 393, texW, texH};
                SDL_RenderCopy(renderer, textMenuSelect_texture, NULL, &textMenuSelect_dest);
                //Set commands mode (Arrow)
                gameCommands = 0;
            }
            if (selectedSettingsMenu_commands == 1) {
                 // Button ZQSD keyboard text
                SDL_Surface * textMenuSelect_surface = TTF_RenderText_Blended(fontMenu, "ZQSD (Clavier)", color);
                SDL_Texture * textMenuSelect_texture = SDL_CreateTextureFromSurface(renderer,textMenuSelect_surface);
                SDL_QueryTexture(textMenuSelect_texture, NULL, NULL, &texW, &texH);
                SDL_Rect textMenuSelect_dest = { 210, 393, texW, texH};
                SDL_RenderCopy(renderer, textMenuSelect_texture, NULL, &textMenuSelect_dest);
                //Set commands mode (ZQSD)
                gameCommands = 1;
            }

            // Button back text
            SDL_Surface * textMenuQuit_surface = TTF_RenderText_Blended(fontMenu, "Retour", color);
            SDL_Texture * textMenuQuit_texture = SDL_CreateTextureFromSurface(renderer,textMenuQuit_surface);
            SDL_QueryTexture(textMenuQuit_texture, NULL, NULL, &texW, &texH);
            SDL_Rect textMenuQuit_dest = { 255, 477, texW, texH};
            SDL_RenderCopy(renderer, textMenuQuit_texture, NULL, &textMenuQuit_dest);

            // Left/Right arrows textures
            SDL_RenderCopy(renderer, arrowLeft, NULL, &arrowMenuLeft1_dst);
            SDL_RenderCopy(renderer, arrowRight, NULL, &arrowMenuRight1_dst);
            SDL_RenderCopy(renderer, arrowLeft, NULL, &arrowMenuLeft2_dst);
            SDL_RenderCopy(renderer, arrowRight, NULL, &arrowMenuRight2_dst);

            SDL_RenderPresent(renderer);

        }


	}

    // Clearing renderer from main menu
    SDL_RenderClear(renderer);

	// Initializing 2D table
	float** table2D;
	table2D = (float**)malloc(map_width * sizeof(float*));

	for (dim_allocated; dim_allocated < map_width; dim_allocated++) {
		table2D[dim_allocated] = (float*)malloc(map_height * sizeof(float));
	}

    // Map loading
    LoadMap(SelectedLevel,table2D,dim_allocated,map_width,map_height);

    // Setting the background
	SDL_RenderCopy(renderer, background, NULL, NULL);

    // 1st fruit drawing
	DrawFruits(&apple,apple_texture,table2D,map_width,map_height,tile_width,tile_height,renderer,&snake,tailX,tailY);

    ////////////////////////////////
	// Main Game loop
    while (!gameRunning)
    {
        // Snake head position
        tailX[0] = snake.position.x;
        tailY[0] = snake.position.y;

        // Snake body variables
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
                    // Arrows command mode
                    case SDLK_UP:
                        if(gameCommands == 0) {
                            if(snake.direction.current != 4 && snake.direction.current != 1) {
                                snake.direction.current = 1;
                                snake.rotation.x = snake.position.x;
                                snake.rotation.y = snake.position.y;
                                snake_head_clip = {90, 0, 30, 30};
                                snake_body_clip ={60, 30, 30, 30};
                                snake_tail_clip ={0, 60, 30, 30};
                            }
                        }
                        break;
                    case SDLK_LEFT:
                        if(gameCommands == 0) {
                            if(snake.direction.current != 3 && snake.direction.current != 2) {
                                snake.direction.current = 2;
                                snake.rotation.x = snake.position.x;
                                snake.rotation.y = snake.position.y;
                                snake_head_clip = {90, 90, 30, 30};
                                snake_body_clip ={60, 0, 30, 30};
                                snake_tail_clip ={60, 60, 30, 30};
                            }
                        }
                        break;
                    case SDLK_RIGHT:
                         if(gameCommands == 0) {
                             if(snake.direction.current != 2 && snake.direction.current != 3) {
                                snake.direction.current = 3;
                                snake.rotation.x = snake.position.x;
                                snake.rotation.y = snake.position.y;
                                snake_head_clip = {90, 30, 30, 30};
                                snake_body_clip ={60, 0, 30, 30};
                                snake_tail_clip ={0, 90, 30, 30};
                                break;
                            }
                        }
                        break;
                    case SDLK_DOWN:
                          if(gameCommands == 0) {
                              if(snake.direction.current != 1 && snake.direction.current != 4) {
                                snake.direction.current = 4;
                                snake.rotation.x = snake.position.x;
                                snake.rotation.y = snake.position.y;
                                snake_head_clip = {90, 60, 30, 30};
                                snake_body_clip ={60, 30, 30, 30};
                                snake_tail_clip ={30, 60, 30, 30};
                                break;
                            }
                        }
                        break;
                    // ZQSD command mode
                    case SDLK_z:
                        if(gameCommands == 1) {
                            if(snake.direction.current != 4 && snake.direction.current != 1) {
                                snake.direction.current = 1;
                                snake.rotation.x = snake.position.x;
                                snake.rotation.y = snake.position.y;
                                snake_head_clip = {90, 0, 30, 30};
                                snake_body_clip ={60, 30, 30, 30};
                                snake_tail_clip ={0, 60, 30, 30};
                            }
                        }
                        break;
                    case SDLK_q:
                        if(gameCommands == 1) {
                            if(snake.direction.current != 3 && snake.direction.current != 2) {
                                snake.direction.current = 2;
                                snake.rotation.x = snake.position.x;
                                snake.rotation.y = snake.position.y;
                                snake_head_clip = {90, 90, 30, 30};
                                snake_body_clip ={60, 0, 30, 30};
                                snake_tail_clip ={60, 60, 30, 30};
                            }
                        }
                        break;
                    case SDLK_d:
                         if(gameCommands == 1) {
                             if(snake.direction.current != 2 && snake.direction.current != 3) {
                                snake.direction.current = 3;
                                snake.rotation.x = snake.position.x;
                                snake.rotation.y = snake.position.y;
                                snake_head_clip = {90, 30, 30, 30};
                                snake_body_clip ={60, 0, 30, 30};
                                snake_tail_clip ={0, 90, 30, 30};
                                break;
                            }
                        }
                        break;
                    case SDLK_s:
                           if(gameCommands == 1) {
                              if(snake.direction.current != 1 && snake.direction.current != 4) {
                                snake.direction.current = 4;
                                snake.rotation.x = snake.position.x;
                                snake.rotation.y = snake.position.y;
                                snake_head_clip = {90, 60, 30, 30};
                                snake_body_clip ={60, 30, 30, 30};
                                snake_tail_clip ={30, 60, 30, 30};
                                break;
                            }
                        }
                        break;
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
        snake.score = ((snake.length-2)*100);
        sprintf(score, "Score : %d", + snake.score);

        // Rendering player's score
        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);

        SDL_Surface * scoreSurface = TTF_RenderText_Blended(font, score, color);
        SDL_Texture * scoreTexture = SDL_CreateTextureFromSurface(renderer,scoreSurface);

        SDL_QueryTexture(scoreTexture, NULL, NULL, &texW, &texH);
        SDL_Rect score_dest = { 5, 605, texW, texH};

        SDL_RenderCopy(renderer, scoreTexture, NULL, &score_dest);

        SDL_RenderPresent(renderer);

        SDL_Delay(snake.speed);

        // In Game Mode = 1 (Normal) when you reach the score specified the game jump to the next level.
        if ((gameMode == 1) && (snake.score == 500) && (SelectedLevel < 6)) {
            SelectedLevel = SelectedLevel+1;
            snake.position.x = 5;
            snake.position.y = 5;
            snake.direction.current = 0;
            snake.length = 2;
            snake_head_clip ={90, 30, 30, 30};
            // Clearing renderer from GzmeOver screen
            SDL_RenderClear(renderer);
            // Map loading
            LoadMap(SelectedLevel,table2D,dim_allocated,map_width,map_height);
            // Setting the background
            SDL_RenderCopy(renderer, background, NULL, NULL);
            // 1st fruit drawing
            DrawFruits(&apple,apple_texture,table2D,map_width,map_height,tile_width,tile_height,renderer,&snake,tailX,tailY);
            SDL_Delay(snake.speed);
        }

        ////////////////////////////////
        // Game Over loop
        while(gameOver) {

            // Clearing renderer from game screen
            SDL_RenderClear(renderer);

            SDL_Rect buttonMenu5_dst = { 114, 385, 372, 56};
            SDL_Rect buttonMenu6_dst = { 114, 472, 372, 56};

            // GameOver background
            SDL_RenderCopy(renderer, gameOver_bg, NULL, NULL);

            // GameOver button
            SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu5_dst);
            SDL_RenderCopy(renderer, menuButton, NULL, &buttonMenu6_dst);

            // Retry button selected
            if(selectedGameOverMenu == 0) {
                SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu5_dst);
            }
            // Quit button selected
            else if (selectedGameOverMenu == 1) {
                SDL_RenderCopy(renderer, menuButton_selected, NULL, &buttonMenu6_dst);
            }
            else {}

            // Button Restart
            SDL_Surface * textGameOver_Restart_surface = TTF_RenderText_Blended(fontMenu, "Rejouer", color);
            SDL_Texture * textGameOver_Restart_texture = SDL_CreateTextureFromSurface(renderer,textGameOver_Restart_surface);
            SDL_QueryTexture(textGameOver_Restart_texture, NULL, NULL, &texW, &texH);
            SDL_Rect textGameOver_Restart_dest = { 250, 395, texW, texH};
            SDL_RenderCopy(renderer, textGameOver_Restart_texture, NULL, &textGameOver_Restart_dest);

            // Button Quit
            SDL_Surface * textGameOver_Quit_surface = TTF_RenderText_Blended(fontMenu, "Quitter", color);
            SDL_Texture * textGameOver_Quit_texture = SDL_CreateTextureFromSurface(renderer,textGameOver_Quit_surface);
            SDL_QueryTexture(textGameOver_Quit_texture, NULL, NULL, &texW, &texH);
            SDL_Rect textGameOver_Quit_dest = { 250, 480, texW, texH};
            SDL_RenderCopy(renderer, textGameOver_Quit_texture, NULL, &textGameOver_Quit_dest);

            SDL_RenderPresent(renderer);
            SDL_WaitEvent(&event);
            switch(event.type)
            {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: SDL_Quit(); return 0; break;
                    case SDLK_UP :
                        if(gameCommands == 0) {
                            if(selectedGameOverMenu == 1) {
                               selectedGameOverMenu--;
                            }
                            else {
                                    selectedGameOverMenu=1;
                            }
                        }
                        break;
                    case SDLK_DOWN :
                        if(gameCommands == 0) {
                            if(selectedGameOverMenu == 0) {
                                selectedGameOverMenu++;
                            }
                            else {
                                    selectedGameOverMenu=0;
                            }
                        }
                        break;
                     case SDLK_z :
                        if(gameCommands == 1) {
                            if(selectedGameOverMenu == 1) {
                               selectedGameOverMenu--;
                            }
                            else {
                                    selectedGameOverMenu=1;
                            }
                        }
                        break;
                    case SDLK_s :
                        if(gameCommands == 1) {
                            if(selectedGameOverMenu == 0) {
                                selectedGameOverMenu++;
                            }
                            else {
                                    selectedGameOverMenu=0;
                            }
                        }
                        break;
                    case SDLK_RETURN :
                        if(selectedGameOverMenu == 0) {
                            gameOver = false;
                            if(gameMode == 1) {
                                SelectedLevel = 1;
                            }
                            snake.position.x = 5;
                            snake.position.y = 5;
                            snake.direction.current = 0;
                            snake.length = 2;
                            snake_head_clip ={90, 30, 30, 30};
                            // Clearing renderer from GzmeOver screen
                            SDL_RenderClear(renderer);
                            // Map loading
                            LoadMap(SelectedLevel,table2D,dim_allocated,map_width,map_height);
                            // Setting the background
                            SDL_RenderCopy(renderer, background, NULL, NULL);
                            // 1st fruit drawing
                            DrawFruits(&apple,apple_texture,table2D,map_width,map_height,tile_width,tile_height,renderer,&snake,tailX,tailY);


                        }
                        else if (selectedGameOverMenu == 1) {
                            gameRunning = true;
                            gameOver = false;
                        }
                        else {}
                     case SDLK_KP_ENTER :
                         if(selectedGameOverMenu == 0) {
                            gameOver = false;
                            if(gameMode == 1) {
                                SelectedLevel = 1;
                            }
                            snake.position.x = 5;
                            snake.position.y = 5;
                            snake.direction.current = 0;
                            snake.length = 2;
                            snake_head_clip ={90, 30, 30, 30};
                            // Clearing renderer from GzmeOver screen
                            SDL_RenderClear(renderer);
                            // Map loading
                            LoadMap(SelectedLevel,table2D,dim_allocated,map_width,map_height);
                            // Setting the background
                            SDL_RenderCopy(renderer, background, NULL, NULL);
                            // 1st fruit drawing
                            DrawFruits(&apple,apple_texture,table2D,map_width,map_height,tile_width,tile_height,renderer,&snake,tailX,tailY);

                        }
                        else if (selectedGameOverMenu == 1) {
                            gameRunning = true;
                            gameOver = false;
                        }
                        else {}
                }
            }
        }
    }
    // Free memory
    free(table2D);

	SDL_DestroyWindow(window);

	TTF_Quit();
	SDL_Quit();

	return 0;


}
