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

void Afficher(SDL_Surface* screen, SDL_Texture* wall, float** table, int map_width, int map_height, SDL_Renderer * sdlRenderer)
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
	// Initialisation de la SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	// Fenetre du jeu
	SDL_Window * fenetre = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);

	// Creation du renderer
	SDL_Renderer * renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) // En cas d'erreur
	{
		printf("Erreur lors de la creation d'un renderer : %s", SDL_GetError());
		return EXIT_FAILURE;
	}

    // Gestion des évènements
    SDL_Event esc;

	// Gestion des textures
	SDL_Surface* tileset = SDL_LoadBMP("textures/wall.bmp");
	if (!tileset) // En cas d'erreur
	{
		printf("Echec de chargement wall.bmp\n");
		SDL_Quit();
		system("pause");
		exit(-1);
	}
	SDL_Surface* screen = SDL_LoadBMP("textures/bg.bmp");
	if (!screen) // En cas d'erreur
	{
		printf("Echec de chargement bg.bmp\n");
		SDL_Quit();
		system("pause");
		exit(-1);
	}

	SDL_Texture* texture_wall = SDL_CreateTextureFromSurface(renderer, tileset);
	SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, screen);

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

	Afficher(screen, texture_wall, table2D, map_width, map_height, renderer);

    // Liberation de mémoire
    free(table2D);
	SDL_FreeSurface(tileset);
	SDL_FreeSurface(screen);

	// Lancement du rendu
	SDL_RenderPresent(renderer);

	// Appuyer sur echap pour quitter
	int quit;
    while (quit)
    {
        SDL_WaitEvent(&esc);
        switch(esc.type)
        {
            case SDL_KEYDOWN:
                switch(esc.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        SDL_Quit();
                        return 0;
                        break;
                }
                break;
        }
    }



}
