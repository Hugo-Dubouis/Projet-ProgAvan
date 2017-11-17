


///////////////////////////////////////////////////////
// Fruit drawing function
/*
void RandomFruit (SDL_Texture* pomme,float** table, int map_width, int map_height, SDL_Renderer * sdlRenderer)
{
    int randHeight, randWidth;
    randHeight = rand() % map_height;
    randWidth = rand() % map_width;
    SDL_Rect Rect_source;
	SDL_Rect Rect_dest;
	Rect_source.x = 0;
	Rect_source.y = 0;
	Rect_source.w = 30;
	Rect_source.h = 30;
	Rect_dest.w = tile_width;
	Rect_dest.h = tile_height;
			if (table[randWidth][randHeight] != 49)  {
				Rect_dest.x = randWidth*tile_width;
				Rect_dest.y = randHeight*tile_height;
				SDL_RenderCopy(sdlRenderer, pomme, &Rect_source, &Rect_dest);
			}
			else(RandomFruit(pomme,table, map_width,map_height,sdlRenderer))

}
*/
