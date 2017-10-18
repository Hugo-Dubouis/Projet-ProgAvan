#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	SDL_Window* fenetre(0);
	// Initialisation de la SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	// Fenetre du jeu
	fenetre = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

	// Programme principal du jeu

	// On ouvre le fichier nivx.txt
	FILE* ptrFichier = fopen("niv1.txt", "r");

	// Menu (à voir à la fin)
	
	// Pour chaque ligne du fichier texte créer un tableau
	int tab;
	int height, width;
	
	// Dimension 1
	float* table1D = (float*) malloc(width * sizeof(float*));

	// Dimension 2
	float** table2D;
	table2D = (float**)malloc(width * sizeof(float*));
	for (int i = 0;i < width; i++) {
		table2D[i] = (float*)malloc(height*sizeof(float));
	}


	if (ptrFichier == NULL) {
		exit(EXIT_FAILURE);
	}

	int fclose(FILE* ptrFichier);

	// Tempo
	SDL_Delay(3000);
	// Destruction de la fenêtre
	SDL_DestroyWindow(fenetre);
	// On quitte la SDL
	SDL_Quit();
	return 0;
}