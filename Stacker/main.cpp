#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Game.h"

bool quit = false;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

Game* game = nullptr;

bool InitGame() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		std::cout << "Unable to initialize SDL Video: " << SDL_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error em carregar SDL", "Algo deu errado no SDL. Certifique-se que dll deve ser v2 e 86x.", window);
		return false;
	}

	if (TTF_Init() != 0)
	{
		std::cout << "Unable to initialize TTF: " << TTF_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error em carregar SDL_TTF", "Algo deu errado no SDL_TTF. Certifique-se que dll deve ser v2 e 86x.", window);
		return false;
	}

	int imgFlags = IMG_InitFlags();

	if (IMG_Init(imgFlags) & imgFlags)
	{
		std::cout << "Unable to initialize IMG: " << IMG_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error em carregar SDL_Image", "Algo deu errado no SDL_Image. Certifique-se que dll deve ser v2 e 86x.", window);
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error no audio", "Certifique-se de que tem um dispositivo de audio.", window);
		return false;
	}

	window = SDL_CreateWindow("Stacker",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		0);

	if (window == NULL) {
		std::cout << "Unable to set video mode: " << SDL_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error na janela", "Certifique-se de que a resolucao minima da tela é de 768x1364.", window);
		return false;
	}

	SDL_Surface* iconSurface = IMG_Load("assets/sprites/icon.png");
	SDL_SetWindowIcon(window, iconSurface);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	game = new Game();

	if(!game->LoadAssets(renderer, window))
	{
		return false;
	}

	return true;
}

void GameLoop()
{
	SDL_Event event;

	while(!quit)
	{
		game->Input(event, quit);
		game->Update();
		game->Render(window, renderer);
	}
}

void CloseGame() {
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if(InitGame()){
		GameLoop();

		CloseGame();
	}
	return 0;
}
