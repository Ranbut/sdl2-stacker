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

bool CheckTexture(SDL_Texture* texture, std::string name, SDL_Window* window)
{
	if (texture == NULL) {
		std::string assetTextError = "Falha para encontrar ou carregar: " + name;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error ao carregar asset sprite", assetTextError.c_str(), window);
		return false;
	}
	else {
		return true;
	}
}
