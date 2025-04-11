#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <shlobj.h> 

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "GameConstants.h"
#include "LoadTexture.h"
#include "GameBoard.h"
#include "ConfigPanel.h"

class Game
{
private:
	int array_matrix[GAME_ROWS][GAME_COLUMNS];

	//States
	bool playing = false;
	bool inConfig = false;
	uint8_t difficultyLevel = 1;
	int BASE_TIME_INTERVAL = 145;
	float SPEED_INCREASE = 1.00f;
	unsigned int credits = 0;
	unsigned int AWARDS = 0;
	int time_delay = BASE_TIME_INTERVAL;

	uint8_t left_or_right = 1;
	uint8_t current_level = 1;
	uint8_t length = 3;
	uint8_t x_pos = 0;
	bool secondRound = false;
	bool winRound = false;
	bool confirm = false;
	uint8_t underflow_ammt = length - 1;

	//Images
	SDL_Texture* gBlock = nullptr;
	SDL_Texture* gBackground = nullptr;
	SDL_Texture* gPrizeA = nullptr;
	SDL_Texture* gPrizeB = nullptr;
	SDL_Texture* gPanel = nullptr;

	SDL_Texture* gCongrats = nullptr;
	SDL_Texture* gContinue = nullptr;
	SDL_Texture* gWinner = nullptr;

	//Music
	Mix_Music* mMusic = nullptr;

	//Sound
	Mix_Chunk* sStart = nullptr;
	Mix_Chunk* sPlace = nullptr;
	Mix_Chunk* sCredit = nullptr;
	Mix_Chunk* sMidway = nullptr;
	Mix_Chunk* sGameOver = nullptr;
	Mix_Chunk* sWin = nullptr;

	//Text
	TTF_Font* font = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_Color colorText = { 0, 0, 0 };

	GameBoard* board = nullptr;
	ConfigPanel* config = nullptr;

	std::string folderDataPath = "";

	void RenderPrizePanel(SDL_Renderer* renderer);
	void RenderContinueMessage(SDL_Renderer* renderer);
	void RenderWinnerScreen(SDL_Renderer* renderer);
	void PlaceBlock();
	void SaveUpdateData(int dataType);
	int GetNewLength(int level);
	void GameOver();

public:
	Game();
	bool LoadAssets(SDL_Renderer *renderer, SDL_Window* window);
	void Input(SDL_Event &event, bool &quit);
	void Update();
	void Render(SDL_Window* window, SDL_Renderer* renderer);
	void ClearAssets();
};

