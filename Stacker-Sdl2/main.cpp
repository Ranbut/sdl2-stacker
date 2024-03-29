#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_mixer.h>
#include <SDL2\SDL_ttf.h>

#define WINDOW_WIDTH 768
#define WINDOW_HEIGHT 1364

#define GAME_ROWS 25
#define GAME_COLUMNS 5

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

#define CREDITS_LIMIT 99

int array_matrix[GAME_ROWS][GAME_COLUMNS];

//States
bool isPlaying = false;
bool inConfig = false;
uint8_t difficultyLevel = 1;
int BASE_TIME_INTERVAL;
float SPEED_INCREASE;
unsigned int credits = 0;
unsigned int IN = 0; 
unsigned int OUTA = 0; 
unsigned int OUTB = 0; 
unsigned int TOTAL = 0; 
unsigned int AWARDS = 0; 
int time_delay = BASE_TIME_INTERVAL;
uint8_t left_or_right = 1;
uint8_t current_level = 1;
uint8_t length = 3;
uint8_t x_pos = 0;
bool quit = false;
bool secondRound = false;
bool winRound = false;
uint8_t underflow_ammt = length - 1;
int frame = 0;
int frame2 = -14;

//Images
SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *gBlock;
SDL_Texture *gBackground;
SDL_Texture *gPrizeA;
SDL_Texture *gPrizeB;
SDL_Texture *gPanel;
SDL_Texture *gConfigPanel;

SDL_Texture *gConfigBox1;
SDL_Texture *gConfigBox2;
SDL_Texture *gConfigBox3;
SDL_Texture *gConfigBox4;
SDL_Texture *gConfigBox5;

SDL_Texture* gCongrats;
SDL_Texture* gContinue;
SDL_Texture* gWinner;

//Music
Mix_Music *mMusic;

//Sound
Mix_Chunk *sStart;
Mix_Chunk *sPlace;
Mix_Chunk *sCredit;
Mix_Chunk *sMidway;
Mix_Chunk *sGameOver;
Mix_Chunk *sWin;

//Text
TTF_Font* font;
TTF_Font* fontText;
TTF_Font* fontText2;
SDL_Color colorText = { 0, 0, 0 };
SDL_Color colorContinue = { 255, 0, 0 };

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

bool KEYS[322];

class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile( std::string path );

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor( Uint8 red, Uint8 green, Uint8 blue );

	//Set blending
	void setBlendMode( SDL_BlendMode blending );

	//Set alpha modulation
	void setAlpha( Uint8 alpha );

	//Renders texture at given point
	void render( int x, int y, SDL_Rect* clip = NULL );

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

SDL_Rect gSpriteClips[30];
LTexture gSpriteSheetFirework;


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy( renderer, mTexture, clip, &renderQuad );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

class GameBoard
{
public:
	void RenderBoard();
	void UpdateBoard(int x_pos, int length, int level);
	void DeleteBoard();
};

/* Use the array_matrix to  indicate where to blit red sqares on the board.
* Also do some arithmetic to get them to print in the proper place, given
* the size of each block is 45x37 */
void GameBoard::RenderBoard() {
	int i, j;
	SDL_Rect src, dest;

	for (i = 0; i < GAME_ROWS; i++) {
		for (j = 0; j < GAME_COLUMNS ; j++) {
			if (array_matrix[i][j] == 1) {
				src.x = 0;
				src.y = 0;
				src.w = 45; 
				src.h = 37;

				dest.x = j * 48 + 474;
				dest.y = i * 40 + 323;
				dest.w = 45;
				dest.h = 37;

				SDL_RenderCopy(renderer, gBlock, &src, &dest);
			}
		}
	}
}

/* Level is the active row, with level 1 being the bottom. 
* x_pos is the current position of the active blocks on the horizontal level. 
x_pos values:
0 = left side
2 = center
4 = right side
underflow_ammt is the number of columns that go off the side of the screen.
*/
void GameBoard::UpdateBoard(int x_pos, int length, int level) {
	int j;

	if (x_pos < underflow_ammt)
		length = length - (underflow_ammt - x_pos);

	x_pos = MAX(0, x_pos-underflow_ammt);

	for (j = 0; j < GAME_COLUMNS; j++) 
		array_matrix[GAME_ROWS - level][j] = 0;

	for (j = x_pos; j < x_pos + length; j++) {
		array_matrix[GAME_ROWS - level][MIN(j, GAME_COLUMNS-1)] = 1;
	}	
}

GameBoard *Board;

SDL_Texture *LoadTexture(std::string filePath, SDL_Renderer *renderTarget){
	SDL_Texture *texture = nullptr;
	SDL_Surface *surface = IMG_Load(filePath.c_str());

	if(surface == NULL)
		std::cout << "Error in creating surface" << std::endl;
	else{
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if(texture == NULL)
			std::cout << "Error in creating texture" << std::endl;
	}

	SDL_FreeSurface(surface);

	return texture;
}

void RenderContinueMessage(){
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

void RenderPanel(){
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

void RenderConfig(){

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

	std::string sIN = "Entrada: " + std::to_string(IN);
	std::string sOUTA = "Saída A: " + std::to_string(OUTA);
	std::string sOUTB = "Saída B: " + std::to_string(OUTB);
	std::string sTOTAL = "Total: " + std::to_string(TOTAL);

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

void RenderWinnerScreen(){
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

	SDL_Rect* currentClip1 = &gSpriteClips[ frame / 4 ];
	gSpriteSheetFirework.render( ( WINDOW_WIDTH - 900 ) / 2, ( WINDOW_HEIGHT - 1000 ) / 2, currentClip1 );
	gSpriteSheetFirework.render( ( WINDOW_WIDTH + 400 ) / 2, ( WINDOW_HEIGHT - 1000 ) / 2, currentClip1 );

	SDL_Rect* currentClip2 = &gSpriteClips[ frame2 / 4 ];
	gSpriteSheetFirework.render( ( WINDOW_WIDTH - 700 ) / 2, ( WINDOW_HEIGHT - 800 ) / 2, currentClip2 );
	gSpriteSheetFirework.render( ( WINDOW_WIDTH + 200 ) / 2, ( WINDOW_HEIGHT - 800 ) / 2, currentClip2 );

	++frame;
	if( frame / 4 >= 30)
	{
		frame = 0;
	}

	++frame2;
	if( frame2 / 4 >= 30)
	{
		frame2 = 0;
	}
}

/* Checks to see that there are blocks supporting the current level and returns
* the number of blocks that should exist on the next level */
int GetNewLength(int level) {
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

void UpdateDifficult(){

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

	std::cout << "Difficult: " << difficultyLevel << std::endl;
}

void GameOver(){
	Mix_HaltMusic();
	Mix_PlayChannel( -1, sGameOver, 0 );
	isPlaying = false;
	time_delay = BASE_TIME_INTERVAL;
	left_or_right = 1;
	current_level = 0;
	length = 3;
	x_pos = 0;
	underflow_ammt = length - 1;
	secondRound = false;
	winRound = false;
	memset(array_matrix, 0, sizeof(array_matrix));

	//Save
	std::ofstream saveFile;
	std::cout << "Credits: " << credits << std::endl;
	saveFile.open ("save.txt");
	saveFile << "Credits: " << credits;
	saveFile.close();
}

void GameLoop() {
	SDL_Event event;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_KEYDOWN: 
				if (event.key.keysym.sym == SDLK_SPACE) {
					if (current_level == GAME_ROWS && !winRound){
						Mix_HaltMusic();
						Mix_PlayChannel( -1, sWin, 0 );
						winRound = true;
					}

					if (current_level == 17 && !secondRound) {
						secondRound = true;
					}
					else
					{
						if(isPlaying && !inConfig && !winRound)
						{
							length = GetNewLength(current_level);
							underflow_ammt = length - 1;
							Mix_PlayChannel( -1, sPlace, 0 );
							if (length == 0) {
								GameOver();
							}
							current_level++;
							if (length != 0 && current_level == 17)
							{
								Mix_PlayChannel( -1, sMidway, 0 );
							}
							time_delay = time_delay/SPEED_INCREASE;
						}
					}
				}

				if (event.key.keysym.sym == SDLK_d){
					if (current_level == 17 && !secondRound) {
						secondRound = true;
						winRound = true;
					}
				}

				if (event.key.keysym.sym == SDLK_t){
					if (current_level == 17 && winRound) {
						OUTA++;
						winRound = false;
						GameOver();
					}

					if (current_level == GAME_ROWS && winRound) {
						OUTB++;
						winRound = false;
						GameOver();
					}
				}

				if (event.key.keysym.sym == SDLK_1 && credits > 0 && !inConfig && !isPlaying){
					Mix_PlayMusic( mMusic, -1 );
					Mix_PlayChannel( -1, sStart, 0 );
					credits--;
					isPlaying = true;
				}

				if (event.key.keysym.sym == SDLK_5 && credits < CREDITS_LIMIT){
					Mix_PlayChannel( -1, sCredit, 0 );
					credits++;
					IN++;

					//Save
					std::ofstream saveFile;
					std::cout << "Credits: " << credits << std::endl;
					saveFile.open ("save.txt");
					saveFile << "Credits: " << credits;
					saveFile.close();
				}

				if (event.key.keysym.sym == SDLK_l && !isPlaying)
					inConfig = !inConfig;

				if (event.key.keysym.sym == SDLK_SPACE && inConfig && !isPlaying)
				{
					UpdateDifficult();
				}

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
		if (!quit) {

			if (x_pos >= GAME_COLUMNS + (underflow_ammt - 1))
				left_or_right = -1;
			if (x_pos <= 0) 
				left_or_right = 1;

			SDL_RenderCopy(renderer, gBackground, 0, 0);

			if(inConfig){
				RenderConfig();
			}
			else{
				RenderPanel();
			}

			if (current_level == 17 && !secondRound) {
				RenderContinueMessage();
				Board->RenderBoard();
			}
			else
			{
				if (isPlaying && !winRound)
				{
					Board->UpdateBoard(x_pos, length, current_level);
					Board->RenderBoard();
				}
			}

			if(winRound){
				RenderWinnerScreen();
			}

			SDL_RenderPresent(renderer);
			x_pos = x_pos + left_or_right;
			SDL_Delay(time_delay);
			SDL_UpdateWindowSurface(window);

		}

		SDL_RenderClear(renderer);
	}
}

bool InitGame(){

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		std::cout << "Unable to initialize SDL Video: " << SDL_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() != 0)
	{
		std::cout << "Unable to initialize TTF: " << TTF_GetError() << std::endl;
		return false;
	}

	int imgFlags = IMG_InitFlags();

	if (IMG_Init(imgFlags) & imgFlags)
	{
		std::cout << "Unable to initialize IMG: " << IMG_GetError() << std::endl;
		return false;
	}

	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return false;
	}

	for(int i = 0; i < 322; i++) { // init them all to false
		KEYS[i] = false;
	}

	window = SDL_CreateWindow("Stacker",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		0);

	if (window == NULL) {
		std::cout << "Unable to set video mode: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED );

	gBlock = LoadTexture("square.bmp", renderer);
	gBackground = LoadTexture("background.bmp", renderer);
	gPanel = LoadTexture("game_panel.png", renderer);
	gPrizeA = LoadTexture("prizeA.bmp", renderer);
	gPrizeB = LoadTexture("prizeB.jpg", renderer);
	gConfigPanel = LoadTexture("config_panel.png", renderer);
	gConfigBox1 = LoadTexture("box.png", renderer);
	gConfigBox2 = LoadTexture("box.png", renderer);
	gConfigBox3 = LoadTexture("box.png", renderer);
	gConfigBox4 = LoadTexture("box.png", renderer);
	gConfigBox5 = LoadTexture("box.png", renderer);
	gConfigBox5 = LoadTexture("box.png", renderer);
	gCongrats = LoadTexture("message_congrats.png", renderer);
	gContinue = LoadTexture("message1.png", renderer);
	gWinner = LoadTexture("message2.png", renderer);

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
	if (gConfigPanel == NULL) {
		std::cout << "Unable to load config_panel" << std::endl;
		return false;
	}
	if (gConfigBox1 == NULL) {
		std::cout << "Unable to load box1" << std::endl;
		return false;
	}
	if (gConfigBox2 == NULL) {
		std::cout << "Unable to load box2" << std::endl;
		return false;
	}
	if (gConfigBox3 == NULL) {
		std::cout << "Unable to load box3" << std::endl;
		return false;
	}
	if (gConfigBox4 == NULL) {
		std::cout << "Unable to load box4" << std::endl;
		return false;
	}
	if (gConfigBox5 == NULL) {
		std::cout << "Unable to load box5" << std::endl;
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

	if (!gSpriteSheetFirework.loadFromFile("firework.png")) {
		std::cout << "Unable to load firework" << std::endl;
		return false;
	}
	else
	{
		for (int x = 0; x < 6; x++)
		{
			for (int y = 0; y < 5; y++)
			{
				gSpriteClips[x * y].x = 256 * x;
				gSpriteClips[x * y].y = 256 * y;
				gSpriteClips[x * y].w = 256;
				gSpriteClips[x * y].h = 256;
			}
		}
	}

    mMusic = Mix_LoadMUS( "overworld.mp3" );
    if( mMusic == NULL )
    {
		std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
    }
	sStart = Mix_LoadWAV( "start.wav");
	if( sStart == NULL )
	{
		std::cout << "Failed to load start sound effect! SDL_mixer Error: " << Mix_GetError() <<std::endl;
		return false;
	}
	sPlace = Mix_LoadWAV( "place.wav");
	if( sPlace == NULL )
	{
		std::cout << "Failed to load place sound effect! SDL_mixer Error: " << Mix_GetError() <<std::endl;
		return false;
	}
	sCredit = Mix_LoadWAV( "credit.wav");
	if( sCredit == NULL )
	{
		std::cout << "Failed to load credit sound effect! SDL_mixer Error: " << Mix_GetError() <<std::endl;
		return false;
	}
	sMidway = Mix_LoadWAV( "midway.wav");
	if( sCredit == NULL )
	{
		std::cout << "Failed to load midway sound effect! SDL_mixer Error: " << Mix_GetError() <<std::endl;
		return false;
	}
	sGameOver = Mix_LoadWAV( "game_over.wav");
	if( sGameOver == NULL )
	{
		std::cout << "Failed to load game_over sound effect! SDL_mixer Error: " << Mix_GetError() <<std::endl;
		return false;
	}
	sWin = Mix_LoadWAV( "win.wav");
	if( sWin == NULL )
	{
		std::cout << "Failed to load win sound effect! SDL_mixer Error: " << Mix_GetError() <<std::endl;
		return false;
	}

	font = TTF_OpenFont("font.ttf", 40);
	if (font == NULL) {
		// Handle error
	}

	fontText = TTF_OpenFont("font.ttf", 30);
	if (fontText == NULL) {
		// Handle error
	}

	fontText2 = TTF_OpenFont("font.ttf", 25);
	if (fontText == NULL) {
		// Handle error
	}

	surface = TTF_RenderText_Solid(font, "00", colorText);
	if (surface == NULL) {
		// Handle error
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {
		// Handle error
	}

	surfaceLevel1 = TTF_RenderText_Solid(font, "1", colorText);
	if (surfaceLevel1 == NULL) {
		// Handle error
	}

	textureLevel1 = SDL_CreateTextureFromSurface(renderer, surfaceLevel1);
	if (textureLevel1 == NULL) {
		// Handle error
	}

	surfaceLevel2 = TTF_RenderText_Solid(font, "2", colorText);
	if (surfaceLevel2 == NULL) {
		// Handle error
	}

	textureLevel2 = SDL_CreateTextureFromSurface(renderer, surfaceLevel2);
	if (textureLevel2 == NULL) {
		// Handle error
	}

	surfaceLevel3 = TTF_RenderText_Solid(font, "3", colorText);
	if (surfaceLevel3 == NULL) {
		// Handle error
	}

	textureLevel3 = SDL_CreateTextureFromSurface(renderer, surfaceLevel3);
	if (textureLevel3 == NULL) {
		// Handle error
	}

	surfaceLevel4 = TTF_RenderText_Solid(font, "4", colorText);
	if (surfaceLevel4 == NULL) {
		// Handle error
	}

	textureLevel4 = SDL_CreateTextureFromSurface(renderer, surfaceLevel4);
	if (textureLevel4 == NULL) {
		// Handle error
	}

	surfaceLevel5 = TTF_RenderText_Solid(font, "5", colorText);
	if (surfaceLevel5 == NULL) {
		// Handle error
	}

	textureLevel5 = SDL_CreateTextureFromSurface(renderer, surfaceLevel5);
	if (textureLevel5 == NULL) {
		// Handle error
	}

	surfaceMenu = TTF_RenderText_Solid(fontText, "Contador Geral", colorText);
	if (surfaceMenu == NULL) {
		// Handle error
	}

	textureMenu = SDL_CreateTextureFromSurface(renderer, surfaceMenu);
	if (textureMenu == NULL) {
		// Handle error
	}

	surfaceIN = TTF_RenderText_Solid(fontText2, "Entrada: ", colorText);
	if (surfaceIN == NULL) {
		// Handle error
	}

	textureIN = SDL_CreateTextureFromSurface(renderer, surfaceIN);
	if (textureIN == NULL) {
		// Handle error
	}

	surfaceOUTA = TTF_RenderText_Solid(fontText2, "Saída A: ", colorText);
	if (surfaceOUTA == NULL) {
		// Handle error
	}

	textureOUTA = SDL_CreateTextureFromSurface(renderer, surfaceOUTA);
	if (textureOUTA == NULL) {
		// Handle error
	}

	surfaceOUTB = TTF_RenderText_Solid(fontText2, "Saída B: ", colorText);
	if (surfaceOUTB == NULL) {
		// Handle error
	}

	textureOUTB = SDL_CreateTextureFromSurface(renderer, surfaceOUTB);
	if (textureOUTB == NULL) {
		// Handle error
	}

	surfaceTotal = TTF_RenderText_Solid(fontText2, "Total: ", colorText);
	if (surfaceTotal == NULL) {
		// Handle error
	}

	textureTotal = SDL_CreateTextureFromSurface(renderer, surfaceTotal);
	if (textureTotal == NULL) {
		// Handle error
	}

	surfaceMenu = TTF_RenderText_Solid(fontText, "Contador Geral", colorText);
	if (surfaceMenu == NULL) {
		// Handle error
	}

	textureMenu = SDL_CreateTextureFromSurface(renderer, surfaceMenu);
	if (textureMenu == NULL) {
		// Handle error
	}

	surfaceMenu2 = TTF_RenderText_Solid(fontText, "Dificuldade:", colorText);
	if (surfaceMenu2 == NULL) {
		// Handle error
	}

	textureMenu2 = SDL_CreateTextureFromSurface(renderer, surfaceMenu2);
	if (textureMenu2 == NULL) {
		// Handle error
	}

	std::string configText;

	std::ifstream saveFile("save.txt");

	if (saveFile == NULL)
	{
		std::cout << "No saved configurations was found. Creating configuration file..." << std::endl;
		std::ofstream saveFile;
		saveFile.open ("save.txt");
		saveFile << "Creditos: " << credits;
		saveFile.close();
	}
	else
	{

		while (getline (saveFile, configText)) {
			std::cout << "Loaded saved configurations and credits." << std::endl;
		}

		saveFile.close(); 

		std::string target;

		for (char c : configText) {
			if (std::isdigit(c)) target += c;
		}

		credits = std::stoi(target);
	}

	UpdateDifficult();

	return true;
}

void CloseGame(){
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(gBlock);
	SDL_DestroyTexture(gBackground);
	SDL_DestroyTexture(gPrizeA);
	SDL_DestroyTexture(gPrizeB);
	SDL_DestroyTexture(gPanel);
	SDL_DestroyTexture(gConfigPanel);
	SDL_DestroyTexture(gConfigBox1);
	SDL_DestroyTexture(gConfigBox2);
	SDL_DestroyTexture(gConfigBox3);
	SDL_DestroyTexture(gConfigBox4);
	SDL_DestroyTexture(gConfigBox5);
	SDL_DestroyTexture(gCongrats);
	SDL_DestroyTexture(gContinue);
	SDL_DestroyTexture(gWinner);

	window = nullptr;

	Mix_FreeMusic(mMusic);

	Mix_FreeChunk(sStart);
	Mix_FreeChunk(sPlace);
	Mix_FreeChunk(sCredit);
	Mix_FreeChunk(sMidway);
	Mix_FreeChunk(sGameOver);
	Mix_FreeChunk(sWin);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

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

	TTF_CloseFont(font);
	TTF_CloseFont(fontText);
	TTF_CloseFont(fontText2);

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	if(InitGame()){

		GameLoop();

		CloseGame();
	}

	return 0;
}
