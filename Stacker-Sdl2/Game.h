#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <unordered_map>
#include <filesystem>
#include <vector>

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
	uint8_t underflow_ammt = length - 1;

	//Images
	SDL_Texture* gBlock;
	SDL_Texture* gBackground;
	SDL_Texture* gPrizeA;
	SDL_Texture* gPrizeB;
	SDL_Texture* gPanel;

	SDL_Texture* gCongrats;
	SDL_Texture* gContinue;
	SDL_Texture* gWinner;

	//Music
	Mix_Music* mMusic;

	//Sound
	Mix_Chunk* sStart;
	Mix_Chunk* sPlace;
	Mix_Chunk* sCredit;
	Mix_Chunk* sMidway;
	Mix_Chunk* sGameOver;
	Mix_Chunk* sWin;

	//Text
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Color colorText = { 0, 0, 0 };

	GameBoard *board;
	ConfigPanel *config;

	void RenderPrizePanel(SDL_Renderer* renderer);
	void RenderContinueMessage(SDL_Renderer* renderer);
	void RenderWinnerScreen(SDL_Renderer* renderer);
	int GetNewLength(int level);
	void GameOver();

public:
	Game();
	bool LoadAssets(SDL_Renderer *renderer);
	void Input(SDL_Event &event, bool &quit);
	void Update();
	void Render(SDL_Window* window, SDL_Renderer* renderer);
	void ClearAssets();
};

