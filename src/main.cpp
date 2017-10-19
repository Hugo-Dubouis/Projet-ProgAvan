#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
	SDL_Window* fenetre(0);
	// Initialisation de la SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	// Fenetre du jeu
	fenetre = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

	// Programme principal du jeu

	// On ouvre le fichier nivx.txt
	

	// Menu (à voir à la fin)
	

	// Pour chaque ligne du fichier texte créer un tableau
	int tab;
	int height = 20;
	int width = 20;

	// Dimension 2
	float** table2D;
	table2D = (float**)malloc(width * sizeof(float*));
	for (int i = 0;i < width; i++) {
		table2D[i] = (float*)malloc(height * sizeof(float));
	}

	//Map
	char map[21][21];
	// Ouverture du fichier Map
	std::ifstream MapFile("niv1.txt");

	// Ouverture de la map avec succes
	if (MapFile)
	{
		for (int height = 0; height < 21; ++height)
		{
			// read current row int map array

			MapFile.read(map[height], 21);

			// print current row just to check

			for (int width = 0; width < 21; ++width)
			{
				std::cout << map[height][width];
			}
		}
		std::cout << std::endl;
	}
	MapFile.close();

	/*
	// Boucle pour lire contenu table 2D
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			std::cout << table2D[i][j] << "\n";

	free(table2D);
	*/
/* Si fichier niveau vide
	if (ptrFichier == NULL) {
		exit(EXIT_FAILURE);
	}
	*/
	int fclose(FILE* ptrFichier);

	// Tempo
	SDL_Delay(3000);
	// Destruction de la fenêtre
	SDL_DestroyWindow(fenetre);
	// On quitte la SDL
	SDL_Quit();
	return 0;
}