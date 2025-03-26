#include "Game.h"

Game::Game()
{

}

void Game::RenderPrizePanel(SDL_Renderer* renderer)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = 351;
	src.h = 290;

	dest.x = 55;
	dest.y = 310;
	dest.w = 351;
	dest.h = 290;

	SDL_RenderCopy(renderer, gPanel, 0, 0);
	if (current_level <= 16)
		SDL_RenderCopy(renderer, gPrizeA, &src, &dest);
	else
		SDL_RenderCopy(renderer, gPrizeB, &src, &dest);

	std::string s = std::to_string(credits);
	surface = TTF_RenderText_Solid(font, s.c_str(), colorText);
	SDL_Rect dest_rect = { 292, 1258, surface->w, surface->h };

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
	SDL_DestroyTexture(texture);
}

void Game::RenderContinueMessage(SDL_Renderer* renderer)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = 642;
	src.h = 83;

	dest.x = WINDOW_WIDTH / 3.5f - 155;
	dest.y = WINDOW_HEIGHT / 3.5f + 150;
	dest.w = 642;
	dest.h = 83;

	SDL_RenderCopy(renderer, gContinue, &src, &dest);
}

void Game::RenderWinnerScreen(SDL_Renderer* renderer)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = 481;
	src.h = 83;

	dest.x = WINDOW_WIDTH / 3.5f - 80;
	dest.y = WINDOW_HEIGHT / 3.5f + 150;
	dest.w = 481;
	dest.h = 83;

	SDL_RenderCopy(renderer, gWinner, &src, &dest);
}

int Game::GetNewLength(int level)
{
	int i;
	int length = 0;

	for (i = 0; i < GAME_COLUMNS; i++) {
		if (array_matrix[GAME_ROWS - level][i] == 1) {
			length++;
			if (level != 1) {
				if (array_matrix[GAME_ROWS - (level - 1)][i] != 1) {
					array_matrix[GAME_ROWS - level][i] = 0;
					length--;
				}
			}
		}
		if ((level == 8 && length == 3) || (level == 16 && length == 2)) {
			length--;
		}
	}
	return length;
}

void Game::GameOver()
{
	winRound = false;
	secondRound = false;
	playing = false;

	Mix_HaltMusic();
	Mix_PlayChannel(-1, sGameOver, 0);

	time_delay = BASE_TIME_INTERVAL;
	left_or_right = 1;
	current_level = 0;
	length = 3;
	x_pos = 0;
	underflow_ammt = length - 1;
	memset(array_matrix, 0, sizeof(array_matrix));

	//Save
	std::ofstream saveFile;
	std::cout << "Credits: " << credits << std::endl;
	saveFile.open("save.txt");
	saveFile << "Credits: " << credits;
	saveFile.close();
}

bool Game::LoadAssets(SDL_Renderer* renderer)
{
	gBlock = LoadTexture("square.bmp", renderer);
	gBackground = LoadTexture("background.bmp", renderer);
	gPanel = LoadTexture("game_panel.png", renderer);
	gPrizeA = LoadTexture("prizeA.bmp", renderer);
	gPrizeB = LoadTexture("prizeB.jpg", renderer);
	gCongrats = LoadTexture("message_congrats.png", renderer);
	gContinue = LoadTexture("message1.png", renderer);
	gWinner = LoadTexture("message2.png", renderer);

	font = TTF_OpenFont("font.ttf", 40);
	if (font == NULL) {
		std::cout << "Failed to load font! SDL_TTF Error: " << TTF_GetError() << std::endl;
	}

	if (gBlock == NULL) {
		std::cout << "Unable to load bitmap block" << std::endl;
		return false;
	}
	if (gBackground == NULL) {
		std::cout << "Unable to load bitmap background" << std::endl;
		return false;
	}
	if (gPanel == NULL) {
		std::cout << "Unable to load game_panel" << std::endl;
		return false;
	}
	if (gPrizeA == NULL) {
		std::cout << "Unable to load bitmap prizeA" << std::endl;
		return false;
	}
	if (gPrizeB == NULL) {
		std::cout << "Unable to load bitmap prizeB" << std::endl;
		return false;
	}
	if (gCongrats == NULL) {
		std::cout << "Unable to load congrats message" << std::endl;
		return false;
	}
	if (gContinue == NULL) {
		std::cout << "Unable to load continue message" << std::endl;
		return false;
	}
	if (gWinner == NULL) {
		std::cout << "Unable to load winner message" << std::endl;
		return false;
	}

	mMusic = Mix_LoadMUS("overworld.mp3");
	if (mMusic == NULL)
	{
		std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sStart = Mix_LoadWAV("start.wav");
	if (sStart == NULL)
	{
		std::cout << "Failed to load start sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sPlace = Mix_LoadWAV("place.wav");
	if (sPlace == NULL)
	{
		std::cout << "Failed to load place sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sCredit = Mix_LoadWAV("credit.wav");
	if (sCredit == NULL)
	{
		std::cout << "Failed to load credit sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sMidway = Mix_LoadWAV("midway.wav");
	if (sCredit == NULL)
	{
		std::cout << "Failed to load midway sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sGameOver = Mix_LoadWAV("game_over.wav");
	if (sGameOver == NULL)
	{
		std::cout << "Failed to load game_over sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sWin = Mix_LoadWAV("win.wav");
	if (sWin == NULL)
	{
		std::cout << "Failed to load win sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}

	surface = TTF_RenderText_Solid(font, "00", colorText);
	if (surface == NULL) {
		return false;
	}

	board = new GameBoard();

	config = new ConfigPanel();

	config->LoadAssets(renderer);

	std::string configText;

	std::ifstream saveFile("save.txt");

	if (!saveFile.is_open())
	{
		std::cout << "No saved configurations was found. Creating configuration file..." << std::endl;
		std::ofstream saveFile;
		saveFile.open("save.txt");
		saveFile << "Creditos: " << credits;
		saveFile.close();
	}
	else
	{
		while (getline(saveFile, configText)) {
			std::cout << "Loaded saved configurations and credits." << std::endl;
		}

		saveFile.close();

		std::string target;

		for (char c : configText) {
			if (std::isdigit(c)) target += c;
		}

		credits = std::stoi(target);
	}

	config->UpdateDifficult(inConfig, BASE_TIME_INTERVAL, SPEED_INCREASE);

	return true;
}

void Game::Input(SDL_Event &event, bool &quit)
{
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {

				if (inConfig && !playing) {
					config->UpdateDifficult(inConfig, BASE_TIME_INTERVAL, SPEED_INCREASE);
				}

				if (current_level == GAME_ROWS && !winRound) {
					Mix_HaltMusic();
					Mix_PlayChannel(-1, sWin, 0);
					winRound = true;
				}

				if (current_level == 17 && !secondRound) {
					secondRound = true;
				}
				else
				{
					if (playing && !inConfig && !winRound)
					{
						length = GetNewLength(current_level);
						underflow_ammt = length - 1;
						Mix_PlayChannel(-1, sPlace, 0);
						if (length == 0) {
							GameOver();
						}
						current_level++;
						if (length != 0 && current_level == 17)
						{
							Mix_PlayChannel(-1, sMidway, 0);
						}
						time_delay = time_delay / SPEED_INCREASE;
					}
				}
			}

			else if (event.key.keysym.sym == SDLK_d) {
				if (current_level == 17 && !secondRound) {
					secondRound = true;
					winRound = true;
				}
			}

			else if (event.key.keysym.sym == SDLK_t) {
				if (current_level == 17 && winRound) {
					config->OUTA++;
					GameOver();
				}

				else if (current_level == GAME_ROWS && winRound) {
					config->OUTB++;
					GameOver();
				}
			}

			else if (event.key.keysym.sym == SDLK_1 && credits > 0 && !inConfig && !playing && !winRound &&! secondRound) {
				winRound = false;
				secondRound = false;
				Mix_PlayMusic(mMusic, -1);
				Mix_HaltChannel(-1);
				Mix_PlayChannel(-1, sStart, 0);
				credits--;
				playing = true;
			}

			else if (event.key.keysym.sym == SDLK_5 && credits < CREDITS_LIMIT) {
				Mix_PlayChannel(-1, sCredit, 0);
				credits++;
				config->IN++;

				//Save
				std::ofstream saveFile;
				std::cout << "Credits: " << credits << std::endl;
				saveFile.open("save.txt");
				saveFile << "Credits: " << credits;
				saveFile.close();
			}

			else if (event.key.keysym.sym == SDLK_l && !playing)
				inConfig = !inConfig;

			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
				exit(0);
			}

			break;
		case SDL_QUIT:
			quit = true;
			exit(0);
		default:
			break;
		}
	}
}

void Game::Update()
{
	if (x_pos >= GAME_COLUMNS + (underflow_ammt - 1))
		left_or_right = -1;
	if (x_pos <= 0)
		left_or_right = 1;

	if (current_level == 17 && !secondRound) {

	}
	else
	{
		if (playing && !winRound)
		{
			board->UpdateBoard(underflow_ammt, array_matrix, x_pos, length, current_level);
		}
	}

	x_pos = x_pos + left_or_right;
	SDL_Delay(time_delay);
	std::cout << "isPlaying: " << playing << "\n";
	std::cout << "secondRound: " << secondRound << "\n";
	std::cout << "winRound: " << winRound << "\n";
	std::cout << "inConfig: " << inConfig << "\n";
}

void Game::Render(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, gBackground, 0, 0);

	if (inConfig && !playing) {
		config->Render(renderer);
	}
	else {
		RenderPrizePanel(renderer);
	}

	if (current_level == 17 && !secondRound) {
		RenderContinueMessage(renderer);
		board->RenderBoard(renderer, gBlock, array_matrix);
	}
	else
	{
		if (playing && !winRound && !inConfig)
		{
			board->RenderBoard(renderer, gBlock, array_matrix);
		}
	}

	if (winRound) {
		RenderWinnerScreen(renderer);
	}

	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
}

void Game::ClearAssets()
{
	SDL_DestroyTexture(gBlock);
	SDL_DestroyTexture(gBackground);
	SDL_DestroyTexture(gPrizeA);
	SDL_DestroyTexture(gPrizeB);
	SDL_DestroyTexture(gPanel);
	SDL_DestroyTexture(gCongrats);
	SDL_DestroyTexture(gContinue);
	SDL_DestroyTexture(gWinner);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	Mix_FreeMusic(mMusic);

	Mix_FreeChunk(sStart);
	Mix_FreeChunk(sPlace);
	Mix_FreeChunk(sCredit);
	Mix_FreeChunk(sMidway);
	Mix_FreeChunk(sGameOver);
	Mix_FreeChunk(sWin);

	TTF_CloseFont(font);
}
