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
	FILE* ptrFichier = fopen(const char* "niv1.txt", const char* "r");

	// Pour chaque ligne du fichier texte cr�er un tableau
	int taille, taille1, taille2, i;
	
	float* table1D = malloc(taille * sizeof(float*));
	
	float** table2D = malloc(taille1 * sizeof(float*))

	if (ptrFichier == NULL) {
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < taille1;) 
		table2D[i] = malloc(taille2 * sizeof(float));



		// 

		int fclose(FILE* ptrFichier)
	}

	// Tempo
	SDL_Delay(3000);
	// Destruction de la fen�tre
	SDL_DestroyWindow(fenetre);
	// On quitte la SDL
	SDL_Quit();
	return 0;
}