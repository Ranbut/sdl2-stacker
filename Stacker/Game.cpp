#include "Game.h"

bool CreateFullDirectory(const std::string& fullPath) {
	std::istringstream pathStream(fullPath);
	std::string segment;
	std::string currentPath;

	// Handle drive letter (e.g., C:)
	if (fullPath.size() >= 2 && fullPath[1] == ':') {
		currentPath = fullPath.substr(0, 2);
		pathStream.ignore(2); // Skip "C:"
	}

	while (std::getline(pathStream, segment, '\\')) {
		if (segment.empty()) continue;
		currentPath += "\\" + segment;
		if (GetFileAttributesA(currentPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
			if (!CreateDirectoryA(currentPath.c_str(), NULL)) {
				return false;
			}
		}
	}

	return true;
}

Game::Game()
{

}

void Game::SaveUpdateData(int dataType) {
	std::ofstream saveFile;

	switch (dataType)
	{
	case 1:
		saveFile.open(folderDataPath + "\\credits");
		saveFile << credits;
		break;
	case 2:
		config->in++;
		saveFile.open(folderDataPath + "\\in");
		saveFile << config->in;
		break;
	case 3:
		config->outA++;
		saveFile.open(folderDataPath + "\\outA");
		saveFile << config->outA;
		break;
	case 4:
		config->outB++;
		saveFile.open(folderDataPath + "\\outB");
		saveFile << config->outB;
		break;
	default:
		break;
	}

	saveFile.close();
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

void Game::PlaceBlock()
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
	confirm = false;

	Mix_HaltMusic();
	Mix_PlayChannel(-1, sGameOver, 0);

	time_delay = BASE_TIME_INTERVAL;
	left_or_right = 1;
	current_level = 0;
	length = 3;
	x_pos = 0;
	underflow_ammt = length - 1;
	memset(array_matrix, 0, sizeof(array_matrix));

	SaveUpdateData(1);
}

bool Game::LoadAssets(SDL_Renderer* renderer, SDL_Window* window)
{
	//Load Assets

	std::string gBlockPath = "assets/sprites/square.bmp";
	std::string gBackgroundPath = "assets/sprites/background.bmp";
	std::string gPanelPath = "assets/sprites/game_panel.png";
	std::string gCongratsPath = "assets/sprites/message_congrats.png";
	std::string gContinuePath = "assets/sprites/message1.png";
	std::string gWinnerPath = "assets/sprites/message2.png";

	std::string gPrizeAPath = "premios/prizeA.bmp";
	std::string gPrizeBPath = "premios/prizeB.jpg";

	gBlock = LoadTexture(gBlockPath, renderer);
	gBackground = LoadTexture(gBackgroundPath, renderer);
	gPanel = LoadTexture(gPanelPath, renderer);
	gCongrats = LoadTexture(gCongratsPath, renderer);
	gContinue = LoadTexture(gContinuePath, renderer);
	gWinner = LoadTexture(gWinnerPath, renderer);

	gPrizeA = LoadTexture(gPrizeAPath, renderer);
	gPrizeB = LoadTexture(gPrizeBPath, renderer);

	font = TTF_OpenFont("assets/fonts/font.ttf", 40);
	if (font == NULL) {
		std::cout << "Failed to load font! SDL_TTF Error: " << TTF_GetError() << std::endl;
	}

	//Check Textures
	if (!CheckTexture(gBlock, gBlockPath, window)) {
		return false;
	}

	if (!CheckTexture(gBackground, gBackgroundPath, window)) {
		return false;
	}

	if (!CheckTexture(gPanel, gPanelPath, window)) {
		return false;
	}

	if (!CheckTexture(gCongrats, gCongratsPath, window)) {
		return false;
	}

	if (!CheckTexture(gContinue, gContinuePath, window)) {
		return false;
	}

	if (!CheckTexture(gWinner, gWinnerPath, window)) {
		return false;
	}

	if (!CheckTexture(gPrizeA, gPrizeAPath, window)) {
		return false;
	}

	if (!CheckTexture(gPrizeB, gPrizeBPath, window)) {
		return false;
	}

	mMusic = Mix_LoadMUS("assets/music/overworld.mp3");
	if (mMusic == NULL)
	{
		std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sStart = Mix_LoadWAV("assets/sounds/start.wav");
	if (sStart == NULL)
	{
		std::cout << "Failed to load start sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sPlace = Mix_LoadWAV("assets/sounds/place.wav");
	if (sPlace == NULL)
	{
		std::cout << "Failed to load place sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sCredit = Mix_LoadWAV("assets/sounds/credit.wav");
	if (sCredit == NULL)
	{
		std::cout << "Failed to load credit sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sMidway = Mix_LoadWAV("assets/sounds/midway.wav");
	if (sCredit == NULL)
	{
		std::cout << "Failed to load midway sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sGameOver = Mix_LoadWAV("assets/sounds/game_over.wav");
	if (sGameOver == NULL)
	{
		std::cout << "Failed to load game_over sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	sWin = Mix_LoadWAV("assets/sounds/win.wav");
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

	//Load Variables

	// Get APPDATA path
	char* appdata = nullptr;
	size_t len = 0;

	if (_dupenv_s(&appdata, &len, "APPDATA") != 0 || appdata == nullptr) {
		std::cerr << "Failed to get APPDATA path." << std::endl;
		return 1;
	}

	folderDataPath = std::string(appdata) + "\\Ranbut\\Stacker";
	free(appdata);

	// Create directory if it doesn't exist
	if (!CreateFullDirectory(folderDataPath)) {
		std::cerr << "Failed to create directory." << std::endl;
		return false;
	}

	std::string configText;

	std::ifstream creditFile(folderDataPath + "\\credits");

	if (!creditFile.is_open())
	{
		std::cout << "No saved Credits was found. Creating file....." << std::endl;
		std::ofstream creditFile;
		creditFile.open(folderDataPath + "\\credits");
		creditFile << 0;
		creditFile.close();
	}
	else
	{
		while (getline(creditFile, configText)) {
			std::cout << "Loaded saved Credits." << std::endl;
		}

		creditFile.close();

		std::string target;

		for (char c : configText) {
			if (std::isdigit(c)) target += c;
		}

		credits = std::stoi(target);
	}

	config->LoadAssets(renderer, window, folderDataPath);
	
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
					if (playing && !inConfig)
					{
						PlaceBlock();
					}
					if (confirm) {
						PlaceBlock();
					}
				}

			}

			if (event.key.keysym.sym == SDLK_d) {
				if (current_level == 17 && !secondRound) {
					secondRound = true;
					winRound = true;
				}
			}

			if (event.key.keysym.sym == SDLK_t) {
				if (current_level == 17 && winRound) {
					SaveUpdateData(3);
					confirm = true;
				}
				
				else if (current_level == GAME_ROWS && winRound) {
					SaveUpdateData(4);
					confirm = true;
				}
			}

			if (event.key.keysym.sym == SDLK_1 && credits > 0 && !inConfig && !playing && !winRound &&! secondRound) {
				winRound = false;
				secondRound = false;
				Mix_PlayMusic(mMusic, -1);
				Mix_HaltChannel(-1);
				Mix_PlayChannel(-1, sStart, 0);
				credits--;
				playing = true;
			}

			if (event.key.keysym.sym == SDLK_5 && credits < CREDITS_LIMIT) {
				Mix_PlayChannel(-1, sCredit, 0);
				credits++;
				SaveUpdateData(2);

				//Save
				SaveUpdateData(1);
			}

			if (event.key.keysym.sym == SDLK_l && !playing)
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
