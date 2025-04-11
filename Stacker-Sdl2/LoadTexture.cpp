#include "LoadTexture.h"

SDL_Texture* LoadTexture(std::string filePath, SDL_Renderer* renderTarget) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (surface == NULL) {
		std::cout << "Error in creating surface: " << IMG_GetError() << std::endl;
	}
	else {
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if (texture == NULL)
			std::cout << "Error in creating texture: " << SDL_GetError() << std::endl;

		SDL_FreeSurface(surface);
	}

	return texture;
}