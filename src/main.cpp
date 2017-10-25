#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

// Dimmensions de la map
const int width = 20;
const int height = 20;
int dim_allocated = 0;

int main(int argc, char **argv) {
	// Initialisation de la SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	// Fenetre du jeu
	SDL_Window * fenetre = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);

	// Creation du renderer
	SDL_Renderer * renderer = SDL_CreateRenderer(fenetre, -1, 0);

	// Texture du background
	SDL_Surface * image1 = SDL_LoadBMP("textures/bg.bmp");
	SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, image1);

	// Texture des murs
	SDL_Surface * image2 = SDL_LoadBMP("textures/wall.bmp");
	SDL_Texture * wall = SDL_CreateTextureFromSurface(renderer, image2);

	// Ouverture du fichier de la map
	FILE *fmap;
	fmap = fopen("niv1.txt", "r");

	// Initialisation du tableau en 2D
	float** table2D;
	table2D = (float**)malloc(width * sizeof(float*));

	for (int dim_allocated = 0;dim_allocated < width; dim_allocated++) {
		table2D[dim_allocated] = (float*)malloc(height * sizeof(float));
	}

	// Attribution des valeurs dans le tableau en 2D

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			table2D[i][j] = fgetc(fmap);
			std::cout << table2D[i][j];
		}
	}

	// Exécution du renderer
	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderPresent(renderer);


	/*free(table2D); */

	// Tempo
	SDL_Delay(3000);
	// Destruction de la fenêtre
	SDL_DestroyWindow(fenetre);
	// On quitte la SDL
	SDL_Quit();
	return 0;
}