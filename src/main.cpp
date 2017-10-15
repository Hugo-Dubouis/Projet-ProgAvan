#include <../SDL2/SDL.h>
int main(int argc, char **argv) {
	SDL_Window* fenetre(0);
	// Initialisation de la SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)
	// Fenetre du jeu
	fenetre = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	// Boucle principale du programme

	// Une pause d’execution
	SDL_Delay(3000);
	// Destruction de la fenêtre
	SDL_DestroyWindow(fenetre);
	// On quitte la SDL
	SDL_Quit();
	return 0;
}