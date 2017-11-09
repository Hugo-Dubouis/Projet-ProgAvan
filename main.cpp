#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

// Dimmensions de la map
const int map_width = 20; // Largeur
const int map_height = 21; // Hauteur
int dim_allocated = 0;

// Dimmensions des blocs
const int tile_width = 30;
const int tile_height = 30;

// Fonction afficher

void DrawMap(SDL_Surface* screen, SDL_Texture* wall, float** table, int map_width, int map_height, SDL_Renderer * sdlRenderer)
{
	int i, j;
	SDL_Rect Rect_source;
	SDL_Rect Rect_dest;
	Rect_source.x = 0;
	Rect_source.y = 0;
	Rect_source.w = 30;
	Rect_source.h = 30;
	Rect_dest.w = tile_width;
	Rect_dest.h = tile_height;

	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			if (table[i][j] == 49) {
				Rect_dest.x = i*tile_width;
				Rect_dest.y = j*tile_height;
				SDL_RenderCopy(sdlRenderer, wall, &Rect_source, &Rect_dest);
			}
		}

	}

}

int main(int argc, char **argv) {
	// Setup SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "Error while initializing SDL" << std::endl;
		return 1;
	}

	// Setup window
	SDL_Window * window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		SDL_Quit();
		std::cout << "Error while loading window" << std::endl;
		return 1;
	}

	// Creation du renderer
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) // En cas d'erreur
	{
		printf("Erreur while creating renderer : %s", SDL_GetError());
		return EXIT_FAILURE;
	}

    // Gestion des évènements
    SDL_Event event;

	// Gestion des textures
	SDL_Surface* tileset = SDL_LoadBMP("textures/wall.bmp");
	if (!tileset) // En cas d'erreur
	{
		printf("Loading failed : wall.bmp\n");
		SDL_Quit();
		system("pause");
		exit(-1);
	}
	SDL_Surface* screen = SDL_LoadBMP("textures/bg.bmp");
	if (!screen) // En cas d'erreur
	{
		printf("Loading failed : bg.bmp\n");
		SDL_Quit();
		system("pause");
		exit(-1);
	}
	SDL_Surface* player = SDL_LoadBMP("textures/snake.bmp");
	if (!player) // En cas d'erreur
	{
		printf("Loading failed : snake.bmp\n");
		SDL_Quit();
		system("pause");
		exit(-1);
	}

	// Creation des textures sur la surface
	SDL_Texture* texture_wall = SDL_CreateTextureFromSurface(renderer, tileset);
	SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_Texture * snake = SDL_CreateTextureFromSurface(renderer, player);

    SDL_FreeSurface(tileset);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(player);


	SDL_RenderCopy(renderer, background, NULL, NULL);




	// Ouverture du fichier de la map
	FILE *fmap;
	fmap = fopen("levels/niv2.txt", "r");

	// Initialisation du tableau en 2D
	float** table2D;
	table2D = (float**)malloc(map_width * sizeof(float*));

	for (int dim_allocated = 0;dim_allocated < map_width; dim_allocated++) {
		table2D[dim_allocated] = (float*)malloc(map_height * sizeof(float));
	}

	// Attribution des valeurs dans le tableau en 2D

	int car_actuel;
	int i, j;

	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			car_actuel = fgetc(fmap);
			if(car_actuel != 10 && car_actuel != -1) {
				table2D[i][j] = car_actuel;
				std::cout << table2D[i][j];
			}
		}
	}

	DrawMap(screen, texture_wall, table2D, map_width, map_height, renderer);
    SDL_RenderPresent(renderer);

	// Personnage
	int x = 30;
	int y = 30;



    // Liberation de mémoire
    free(table2D);

	// Lancement du rendu


	// Appuyer sur echap pour quitter
	int quit = 0;
    while (!quit)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: SDL_Quit(); return 0; break;
                    case SDLK_LEFT:  x = x-30; break;
                    case SDLK_RIGHT: x = x+30; break;
                    case SDLK_UP:    y = y-30; break;
                    case SDLK_DOWN:  y = y+30; break;
                }
                break;

        }
        SDL_Rect dstrect = { x, y, 30, 30};
        SDL_RenderCopy(renderer, snake, NULL, &dstrect);
        SDL_RenderPresent(renderer);
    }



}
