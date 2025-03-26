#pragma once

#include <iostream>
#include <string>

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
	TTF_Font* font;
	TTF_Font* fontText;
	TTF_Font* fontText2;
	SDL_Color colorText = { 0, 0, 0 };
	SDL_Color colorContinue = { 255, 0, 0 };

	SDL_Texture* gConfigPanel;
	SDL_Texture* gConfigBox1;
	SDL_Texture* gConfigBox2;
	SDL_Texture* gConfigBox3;
	SDL_Texture* gConfigBox4;
	SDL_Texture* gConfigBox5;

	SDL_Surface* surface;
	SDL_Texture* texture;

	SDL_Surface* surfaceLevel1;
	SDL_Texture* textureLevel1;

	SDL_Surface* surfaceLevel2;
	SDL_Texture* textureLevel2;

	SDL_Surface* surfaceLevel3;
	SDL_Texture* textureLevel3;

	SDL_Surface* surfaceLevel4;
	SDL_Texture* textureLevel4;

	SDL_Surface* surfaceLevel5;
	SDL_Texture* textureLevel5;

	SDL_Surface* surfaceIN;
	SDL_Texture* textureIN;

	SDL_Surface* surfaceOUTA;
	SDL_Texture* textureOUTA;

	SDL_Surface* surfaceOUTB;
	SDL_Texture* textureOUTB;

	SDL_Surface* surfaceTotal;
	SDL_Texture* textureTotal;

	SDL_Surface* surfaceMenu;
	SDL_Texture* textureMenu;

	SDL_Surface* surfaceMenu2;
	SDL_Texture* textureMenu2;

public:
	unsigned int difficultyLevel = 1;
	unsigned int IN = 0;
	unsigned int OUTA = 0;
	unsigned int OUTB = 0;
	unsigned int TOTAL = 0;

	ConfigPanel();
	bool LoadAssets(SDL_Renderer* renderer);
	void Render(SDL_Renderer* renderer);
	void UpdateDifficult(bool inConfig, int BASE_TIME_INTERVAL, float SPEED_INCREASE);
	void ClearAssets();
};