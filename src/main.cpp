#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
	// Initialisation de la SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	// Fenetre du jeu
	SDL_Window * fenetre = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

	// Creation du renderer
	SDL_Renderer * renderer = SDL_CreateRenderer(fenetre, -1, 0);

	// Texture du background
	SDL_Surface * image1 = SDL_LoadBMP("textures/bg.bmp");
	SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, image1);

	// Texture des murs
	SDL_Surface * image2 = SDL_LoadBMP("textures/wall.bmp");
	SDL_Texture * wall = SDL_CreateTextureFromSurface(renderer, image2);

	// Menu (à voir à la fin)
	

	// Pour chaque ligne du fichier texte créer un tableau
	/* int tab;
	int height = 20;
	int width = 20; 

	// Dimension 2
	float** table2D;
	table2D = (float**)malloc(width * sizeof(float*));
	for (int i = 0;i < width; i++) {
		table2D[i] = (float*)malloc(height * sizeof(float));
	}
	*/

	//Map


	// Initialisation de la map (Tableau 20x20)
	char map[20][20];
	// Image de fond

	// Texture des murs

	// Ouverture du fichier Map
	std::ifstream MapFile("niv1.txt");

	// Ouverture de la map avec succes
	if (MapFile)
	{
		for (int height = 0; height < 21; ++height)
		{
			// On lis le contenu de chaque ligne de longueur 20q
			MapFile.read(map[height], 21);


			// Création du background
			SDL_RenderCopy(renderer, background, NULL, NULL);
		
			// Affichage du contenu du tableau dans la console

			for (int width = 0; width < 21; ++width)
			{
				std::cout << map[height][width];
				// Code pour afficher des murs aux coordonées du tableau contenant des 1
				if (map[height][width] == 1) {
					SDL_Rect position;
					position.x = width;
					position.y = height;
					SDL_RenderCopy(renderer, wall, NULL, &position);
				}
				SDL_RenderPresent(renderer);
			}
			
		}
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