#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "GameConstants.h"
#include "LoadTexture.h"

class ConfigPanel
{
private:
	//Text
	TTF_Font* font = nullptr;
	TTF_Font* fontText = nullptr;
	TTF_Font* fontText2 = nullptr;
	SDL_Color colorText = { 0, 0, 0 };
	SDL_Color colorContinue = { 255, 0, 0 };

	SDL_Texture* gConfigPanel = nullptr;
	SDL_Texture* gConfigBox1 = nullptr;
	SDL_Texture* gConfigBox2 = nullptr;
	SDL_Texture* gConfigBox3 = nullptr;
	SDL_Texture* gConfigBox4 = nullptr;
	SDL_Texture* gConfigBox5 = nullptr;

	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;

	SDL_Surface* surfaceLevel1 = nullptr;
	SDL_Texture* textureLevel1 = nullptr;

	SDL_Surface* surfaceLevel2 = nullptr;
	SDL_Texture* textureLevel2 = nullptr;

	SDL_Surface* surfaceLevel3 = nullptr;
	SDL_Texture* textureLevel3 = nullptr;

	SDL_Surface* surfaceLevel4 = nullptr;
	SDL_Texture* textureLevel4 = nullptr;

	SDL_Surface* surfaceLevel5 = nullptr;
	SDL_Texture* textureLevel5 = nullptr;

	SDL_Surface* surfaceIN = nullptr;
	SDL_Texture* textureIN = nullptr;

	SDL_Surface* surfaceOUTA = nullptr;
	SDL_Texture* textureOUTA = nullptr;

	SDL_Surface* surfaceOUTB = nullptr;
	SDL_Texture* textureOUTB = nullptr;

	SDL_Surface* surfaceTotal = nullptr;
	SDL_Texture* textureTotal = nullptr;

	SDL_Surface* surfaceMenu = nullptr;
	SDL_Texture* textureMenu = nullptr;

	SDL_Surface* surfaceMenu2 = nullptr;
	SDL_Texture* textureMenu2 = nullptr;

public:
	unsigned int difficultyLevel = 1;
	unsigned int in = 0;
	unsigned int outA = 0;
	unsigned int outB = 0;
	unsigned int total = 0;

	ConfigPanel();
	bool LoadAssets(SDL_Renderer* renderer,SDL_Window* window, std::string folderDataPath);
	void Render(SDL_Renderer* renderer);
	void UpdateDifficult(bool inConfig, int BASE_TIME_INTERVAL, float SPEED_INCREASE);
	void ClearAssets();
};