#include "ConfigPanel.h"

ConfigPanel::ConfigPanel() 
{

}

bool ConfigPanel::LoadAssets(SDL_Renderer* renderer, SDL_Window* window, std::string folderDataPath)
{

	std::string gConfigPanelPath = "assets/sprites/config_panel.png";
	std::string gConfigBoxPath = "assets/sprites/box.png";

	gConfigPanel = LoadTexture(gConfigPanelPath, renderer);
	gConfigBox1 = LoadTexture(gConfigBoxPath, renderer);
	gConfigBox2 = LoadTexture(gConfigBoxPath, renderer);
	gConfigBox3 = LoadTexture(gConfigBoxPath, renderer);
	gConfigBox4 = LoadTexture(gConfigBoxPath, renderer);
	gConfigBox5 = LoadTexture(gConfigBoxPath, renderer);
	
	//Check Textures
	if (!CheckTexture(gConfigPanel, gConfigPanelPath, window)) {
		return false;
	}

	if (!CheckTexture(gConfigBox1, gConfigBoxPath, window)) {
		return false;
	}

	if (!CheckTexture(gConfigBox2, gConfigBoxPath, window)) {
		return false;
	}

	if (!CheckTexture(gConfigBox3, gConfigBoxPath, window)) {
		return false;
	}

	if (!CheckTexture(gConfigBox4, gConfigBoxPath, window)) {
		return false;
	}

	if (!CheckTexture(gConfigBox5, gConfigBoxPath, window)) {
		return false;
	}

	font = TTF_OpenFont("assets/fonts/font.ttf", 40);
	if (font == NULL) {
		std::cout << "Failed to load font! SDL_TTF Error: " << TTF_GetError() << std::endl;
	}

	fontText = TTF_OpenFont("assets/fonts/font.ttf", 30);
	if (fontText == NULL) {
		std::cout << "Failed to load font! SDL_TTF Error: " << TTF_GetError() << std::endl;
	}

	fontText2 = TTF_OpenFont("assets/fonts/font.ttf", 25);
	if (fontText == NULL) {
		std::cout << "Failed to load font! SDL_TTF Error: " << TTF_GetError() << std::endl;
		return false;
	}

	surfaceLevel1 = TTF_RenderText_Solid(font, "1", colorText);
	if (surfaceLevel1 == NULL) {
		std::cout << "Unable to create level 1 text." << std::endl;
		return false;
	}

	textureLevel1 = SDL_CreateTextureFromSurface(renderer, surfaceLevel1);
	if (textureLevel1 == NULL) {
		std::cout << "Unable to create level 1 text." << std::endl;
		return false;
	}

	surfaceLevel2 = TTF_RenderText_Solid(font, "2", colorText);
	if (surfaceLevel2 == NULL) {
		std::cout << "Unable to create level 2 text." << std::endl;
		return false;
	}

	textureLevel2 = SDL_CreateTextureFromSurface(renderer, surfaceLevel2);
	if (textureLevel2 == NULL) {
		std::cout << "Unable to create level 2 text." << std::endl;
		return false;
	}

	surfaceLevel3 = TTF_RenderText_Solid(font, "3", colorText);
	if (surfaceLevel3 == NULL) {
		std::cout << "Unable to create level 3 text." << std::endl;
		return false;
	}

	textureLevel3 = SDL_CreateTextureFromSurface(renderer, surfaceLevel3);
	if (textureLevel3 == NULL) {
		std::cout << "Unable to create level 3 text." << std::endl;
		return false;
	}

	surfaceLevel4 = TTF_RenderText_Solid(font, "4", colorText);
	if (surfaceLevel4 == NULL) {
		std::cout << "Unable to create level 4 text." << std::endl;
		return false;
	}

	textureLevel4 = SDL_CreateTextureFromSurface(renderer, surfaceLevel4);
	if (textureLevel4 == NULL) {
		std::cout << "Unable to create level 4 text." << std::endl;
		return false;
	}

	surfaceLevel5 = TTF_RenderText_Solid(font, "5", colorText);
	if (surfaceLevel5 == NULL) {
		std::cout << "Unable to create level 5 text." << std::endl;
		return false;
	}

	textureLevel5 = SDL_CreateTextureFromSurface(renderer, surfaceLevel5);
	if (textureLevel5 == NULL) {
		std::cout << "Unable to create level 5 text." << std::endl;
		return false;
	}

	surfaceMenu = TTF_RenderText_Solid(fontText, "Contador Geral", colorText);
	if (surfaceMenu == NULL) {
		std::cout << "Unable to create menu text." << std::endl;
		return false;
	}

	textureMenu = SDL_CreateTextureFromSurface(renderer, surfaceMenu);
	if (textureMenu == NULL) {
		std::cout << "Unable to create menu text." << std::endl;
		return false;
	}

	surfaceIN = TTF_RenderText_Solid(fontText2, "Entrada: ", colorText);
	if (surfaceIN == NULL) {
		std::cout << "Unable to create in text." << std::endl;
		return false;
	}

	textureIN = SDL_CreateTextureFromSurface(renderer, surfaceIN);
	if (textureIN == NULL) {
		std::cout << "Unable to create in text." << std::endl;
		return false;
	}

	surfaceOUTA = TTF_RenderText_Solid(fontText2, "Saída A: ", colorText);
	if (surfaceOUTA == NULL) {
		std::cout << "Unable to create out a text." << std::endl;
		return false;
	}

	textureOUTA = SDL_CreateTextureFromSurface(renderer, surfaceOUTA);
	if (textureOUTA == NULL) {
		std::cout << "Unable to create out a text." << std::endl;
		return false;
	}

	surfaceOUTB = TTF_RenderText_Solid(fontText2, "Saída B: ", colorText);
	if (surfaceOUTB == NULL) {
		std::cout << "Unable to create out b text." << std::endl;
		return false;
	}

	textureOUTB = SDL_CreateTextureFromSurface(renderer, surfaceOUTB);
	if (textureOUTB == NULL) {
		std::cout << "Unable to create out b text." << std::endl;
		return false;
	}

	surfaceTotal = TTF_RenderText_Solid(fontText2, "Total: ", colorText);
	if (surfaceTotal == NULL) {
		std::cout << "Unable to create total text." << std::endl;
		return false;
	}

	textureTotal = SDL_CreateTextureFromSurface(renderer, surfaceTotal);
	if (textureTotal == NULL) {
		std::cout << "Unable to create total text." << std::endl;
		return false;
	}

	surfaceMenu = TTF_RenderText_Solid(fontText, "Contador Geral", colorText);
	if (surfaceMenu == NULL) {
		std::cout << "Unable to create menu text." << std::endl;
		return false;
	}

	textureMenu = SDL_CreateTextureFromSurface(renderer, surfaceMenu);
	if (textureMenu == NULL) {
		std::cout << "Unable to create menu text." << std::endl;
		return false;
	}

	surfaceMenu2 = TTF_RenderText_Solid(fontText, "Dificuldade:", colorText);
	if (surfaceMenu2 == NULL) {
		std::cout << "Unable to create difficult text." << std::endl;
		return false;
	}

	textureMenu2 = SDL_CreateTextureFromSurface(renderer, surfaceMenu2);
	if (textureMenu2 == NULL) {
		std::cout << "Unable to create difficult text." << std::endl;
		return false;
	}

	std::string configText;

	std::ifstream inFile(folderDataPath + "\\in");

	if (!inFile.is_open())
	{
		std::cout << "No saved IN was found. Creating file..." << std::endl;
		std::ofstream inFile;
		inFile.open(folderDataPath + "\\in");
		inFile << 0;
		inFile.close();
	}
	else
	{
		while (getline(inFile, configText)) {
			std::cout << "Loaded saved IN." << std::endl;
		}

		inFile.close();

		std::string target;

		for (char c : configText) {
			if (std::isdigit(c)) target += c;
		}

		in = std::stoi(target);
	}

	std::ifstream outAFile(folderDataPath + "\\outA");

	if (!outAFile.is_open())
	{
		std::cout << "No saved OUT B was found. Creating file..." << std::endl;
		std::ofstream outAFile;
		outAFile.open(folderDataPath + "\\outA");
		outAFile << 0;
		outAFile.close();
	}
	else
	{
		while (getline(outAFile, configText)) {
			std::cout << "Loaded saved OUT A." << std::endl;
		}

		outAFile.close();

		std::string target;

		for (char c : configText) {
			if (std::isdigit(c)) target += c;
		}

		outA = std::stoi(target);
	}

	std::ifstream outBFile(folderDataPath + "\\outB");

	if (!outBFile.is_open())
	{
		std::cout << "No saved OUT A was found. Creating file..." << std::endl;
		std::ofstream outBFile;
		outBFile.open(folderDataPath + "\\outB");
		outBFile << 0;
		outBFile.close();
	}
	else
	{
		while (getline(outBFile, configText)) {
			std::cout << "Loaded saved OUT B." << std::endl;
		}

		outBFile.close();

		std::string target;

		for (char c : configText) {
			if (std::isdigit(c)) target += c;
		}

		outB = std::stoi(target);
	}

	return true;
}

void ConfigPanel::Render(SDL_Renderer* renderer)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = 337;
	src.h = 318;

	dest.x = WINDOW_WIDTH / 3.5f;
	dest.y = WINDOW_HEIGHT / 3.5f;
	dest.w = 337;
	dest.h = 318;

	SDL_RenderCopy(renderer, gConfigPanel, &src, &dest);

	SDL_Rect destMenu;
	SDL_Rect destMenu2;
	SDL_Rect destIN;
	SDL_Rect destOUTA;
	SDL_Rect destOUTB;
	SDL_Rect destTotal;

	destMenu.x = WINDOW_WIDTH / 3.5f + 20;
	destMenu.y = WINDOW_HEIGHT / 3.5f + 10;
	destMenu.w = surfaceMenu->w;
	destMenu.h = surfaceMenu->h;

	destMenu2.x = WINDOW_WIDTH / 3.5f + 20;
	destMenu2.y = WINDOW_HEIGHT / 3.5f + 200;
	destMenu2.w = surfaceMenu2->w;
	destMenu2.h = surfaceMenu2->h;

	std::string sIN = "Entrada: " + std::to_string(in);
	std::string sOUTA = "Saída A: " + std::to_string(outA);
	std::string sOUTB = "Saída B: " + std::to_string(outB);
	std::string sTOTAL = "Total: " + std::to_string(total);

	surfaceIN = TTF_RenderText_Solid(fontText2, sIN.c_str(), colorText);
	surfaceOUTA = TTF_RenderText_Solid(fontText2, sOUTA.c_str(), colorText);
	surfaceOUTB = TTF_RenderText_Solid(fontText2, sOUTB.c_str(), colorText);
	surfaceTotal = TTF_RenderText_Solid(fontText2, sTOTAL.c_str(), colorText);

	destIN.x = WINDOW_WIDTH / 3.5f + 30;
	destIN.y = WINDOW_HEIGHT / 3.5f + 40;
	destIN.w = surfaceIN->w;
	destIN.h = surfaceIN->h;

	destOUTA.x = WINDOW_WIDTH / 3.5f + 30;
	destOUTA.y = WINDOW_HEIGHT / 3.5f + 80;
	destOUTA.w = surfaceOUTA->w;
	destOUTA.h = surfaceOUTA->h;

	destOUTB.x = WINDOW_WIDTH / 3.5f + 30;
	destOUTB.y = WINDOW_HEIGHT / 3.5f + 120;
	destOUTB.w = surfaceOUTB->w;
	destOUTB.h = surfaceOUTB->h;

	destTotal.x = WINDOW_WIDTH / 3.5f + 30;
	destTotal.y = WINDOW_HEIGHT / 3.5f + 160;
	destTotal.w = surfaceTotal->w;
	destTotal.h = surfaceTotal->h;


	textureIN = SDL_CreateTextureFromSurface(renderer, surfaceIN);
	SDL_FreeSurface(surfaceIN);

	textureOUTA = SDL_CreateTextureFromSurface(renderer, surfaceOUTA);
	SDL_FreeSurface(surfaceOUTA);

	textureOUTB = SDL_CreateTextureFromSurface(renderer, surfaceOUTB);
	SDL_FreeSurface(surfaceOUTB);

	textureTotal = SDL_CreateTextureFromSurface(renderer, surfaceTotal);
	SDL_FreeSurface(surfaceTotal);

	SDL_RenderCopy(renderer, textureMenu, NULL, &destMenu);
	SDL_RenderCopy(renderer, textureMenu2, NULL, &destMenu2);

	SDL_RenderCopy(renderer, textureIN, NULL, &destIN);
	SDL_DestroyTexture(textureIN);

	SDL_RenderCopy(renderer, textureOUTA, NULL, &destOUTA);
	SDL_DestroyTexture(textureOUTA);

	SDL_RenderCopy(renderer, textureOUTB, NULL, &destOUTB);
	SDL_DestroyTexture(textureOUTB);

	SDL_RenderCopy(renderer, textureTotal, NULL, &destTotal);
	SDL_DestroyTexture(textureTotal);

	SDL_Rect srcBox1, destBox1;
	SDL_Rect srcBox2, destBox2;
	SDL_Rect srcBox3, destBox3;
	SDL_Rect srcBox4, destBox4;
	SDL_Rect srcBox5, destBox5;

	srcBox1.x = 0;
	srcBox1.y = 0;
	srcBox1.w = 40;
	srcBox1.h = 40;

	destBox1.x = dest.x + (1 * 60 - 30);
	destBox1.y = dest.y + 250;
	destBox1.w = 40;
	destBox1.h = 40;

	srcBox2.x = 0;
	srcBox2.y = 0;
	srcBox2.w = 40;
	srcBox2.h = 40;

	destBox2.x = dest.x + (2 * 60 - 30);
	destBox2.y = dest.y + 250;
	destBox2.w = 40;
	destBox2.h = 40;

	srcBox3.x = 0;
	srcBox3.y = 0;
	srcBox3.w = 40;
	srcBox3.h = 40;

	destBox3.x = dest.x + (3 * 60 - 30);
	destBox3.y = dest.y + 250;
	destBox3.w = 40;
	destBox3.h = 40;

	srcBox4.x = 0;
	srcBox4.y = 0;
	srcBox4.w = 40;
	srcBox4.h = 40;

	destBox4.x = dest.x + (4 * 60 - 30);
	destBox4.y = dest.y + 250;
	destBox4.w = 40;
	destBox4.h = 40;

	srcBox5.x = 0;
	srcBox5.y = 0;
	srcBox5.w = 40;
	srcBox5.h = 40;

	destBox5.x = dest.x + (5 * 60 - 30);
	destBox5.y = dest.y + 250;
	destBox5.w = 40;
	destBox5.h = 40;

	srcBox5.x = 0;
	srcBox5.y = 0;
	srcBox5.w = 40;
	srcBox5.h = 40;

	SDL_RenderCopy(renderer, gConfigBox1, &srcBox1, &destBox1);
	SDL_RenderCopy(renderer, gConfigBox2, &srcBox2, &destBox2);
	SDL_RenderCopy(renderer, gConfigBox3, &srcBox3, &destBox3);
	SDL_RenderCopy(renderer, gConfigBox4, &srcBox4, &destBox4);
	SDL_RenderCopy(renderer, gConfigBox5, &srcBox5, &destBox5);

	switch (difficultyLevel)
	{
	case 1:
		SDL_SetTextureColorMod(gConfigBox1, 255, 0, 0);
		SDL_SetTextureColorMod(gConfigBox2, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox3, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox4, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox5, 255, 255, 255);
		break;
	case 2:
		SDL_SetTextureColorMod(gConfigBox1, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox2, 255, 0, 0);
		SDL_SetTextureColorMod(gConfigBox3, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox4, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox5, 255, 255, 255);
		break;
	case 3:
		SDL_SetTextureColorMod(gConfigBox1, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox2, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox3, 255, 0, 0);
		SDL_SetTextureColorMod(gConfigBox4, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox5, 255, 255, 255);
		break;
	case 4:
		SDL_SetTextureColorMod(gConfigBox1, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox2, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox3, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox4, 255, 0, 0);
		SDL_SetTextureColorMod(gConfigBox5, 255, 255, 255);
		break;
	case 5:
		SDL_SetTextureColorMod(gConfigBox1, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox2, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox3, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox4, 255, 255, 255);
		SDL_SetTextureColorMod(gConfigBox5, 255, 0, 0);
		break;
	}

	for (int i = 1; i < 6; i++)
	{
		SDL_Rect destText;

		destText.x = dest.x + (i * 60 - 20);
		destText.y = dest.y + 250;
		destText.w = surfaceLevel1->w;
		destText.h = surfaceLevel1->h;

		std::string s = std::to_string(i);

		surface = TTF_RenderText_Solid(font, s.c_str(), colorText);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		SDL_RenderCopy(renderer, texture, NULL, &destText);
		SDL_DestroyTexture(texture);
	}
}

void ConfigPanel::UpdateDifficult(bool inConfig, int BASE_TIME_INTERVAL, float SPEED_INCREASE)
{
	if (inConfig)
		difficultyLevel++;

	switch (difficultyLevel)
	{
	case 1:
		BASE_TIME_INTERVAL = 145;
		SPEED_INCREASE = 1.00f;
		break;
	case 2:
		BASE_TIME_INTERVAL = 148;
		SPEED_INCREASE = 1.02f;
		break;
	case 3:
		BASE_TIME_INTERVAL = 150;
		SPEED_INCREASE = 1.03f;
		break;
	case 4:
		BASE_TIME_INTERVAL = 150;
		SPEED_INCREASE = 1.05f;
		break;
	case 5:
		BASE_TIME_INTERVAL = 150;
		SPEED_INCREASE = 1.06f;
		break;
	case 6:
		BASE_TIME_INTERVAL = 145;
		SPEED_INCREASE = 1.00f;
		difficultyLevel = 1;
		break;
	}
}

void ConfigPanel::ClearAssets()
{
	SDL_DestroyTexture(gConfigPanel);
	SDL_DestroyTexture(gConfigBox1);
	SDL_DestroyTexture(gConfigBox2);
	SDL_DestroyTexture(gConfigBox3);
	SDL_DestroyTexture(gConfigBox4);
	SDL_DestroyTexture(gConfigBox5);

	SDL_DestroyTexture(textureLevel1);
	SDL_FreeSurface(surfaceLevel1);

	SDL_DestroyTexture(textureLevel2);
	SDL_FreeSurface(surfaceLevel2);

	SDL_DestroyTexture(textureLevel3);
	SDL_FreeSurface(surfaceLevel3);

	SDL_DestroyTexture(textureLevel4);
	SDL_FreeSurface(surfaceLevel4);

	SDL_DestroyTexture(textureLevel5);
	SDL_FreeSurface(surfaceLevel5);

	SDL_DestroyTexture(textureMenu);
	SDL_FreeSurface(surfaceMenu);

	SDL_DestroyTexture(textureMenu2);
	SDL_FreeSurface(surfaceMenu2);

	SDL_DestroyTexture(textureIN);
	SDL_FreeSurface(surfaceIN);

	SDL_DestroyTexture(textureOUTA);
	SDL_FreeSurface(surfaceOUTA);

	SDL_DestroyTexture(textureOUTB);
	SDL_FreeSurface(surfaceOUTB);

	SDL_DestroyTexture(textureTotal);
	SDL_FreeSurface(surfaceTotal);

	TTF_CloseFont(fontText);
	TTF_CloseFont(fontText2);
}
