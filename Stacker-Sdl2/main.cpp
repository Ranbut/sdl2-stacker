#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_mixer.h>
#include <iostream>
#include <string>

#define BASE_TIME_INTERVAL 150 /* in microseconds */
#define SPEED_INCREASE 1.03
#define GAME_ROWS 26
#define GAME_COLUMNS 5

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

int array_matrix[GAME_ROWS][GAME_COLUMNS];

//States
bool isPlaying = false;
int credits = 0;
int time_delay = BASE_TIME_INTERVAL;
int left_or_right = 1;
int current_level = 1;
int length = 3;
int x_pos = 0;
bool quit = false;
int underflow_ammt = length - 1;

//Images
SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *gBlock;
SDL_Texture *gBackground;
SDL_Texture *gPrize;

//Sound
Mix_Chunk *sStart;
Mix_Chunk *sPlace;
Mix_Chunk *sCredit;

class GameBoard
{
public:
	GameBoard();
	~GameBoard();
	void RenderBoard();
	void UpdateBoard(int x_pos, int length, int level);
	void DeleteBoard();
};

GameBoard::~GameBoard() {}

/* Use the array_matrix to  indicate where to blit red sqares on the board.
 * Also do some arithmetic to get them to print in the proper place, given
 * the size of each block is 65 */
void GameBoard::RenderBoard() {
	int i, j;
    SDL_Rect src, dest;

	for (i = 0; i < GAME_ROWS; i++) {
		for (j = 0; j < GAME_COLUMNS ; j++) {
			if (array_matrix[i][j] == 1) {
				src.x = 0;
				src.y = 0;
				src.w = 65; 
				src.h = 65;

				dest.x = j * 48 + 474;
				dest.y = i * 40 + 282;
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
	SDL_Surface *surface = SDL_LoadBMP(filePath.c_str());

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

void RenderBackground(){
    SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = 351; 
	src.h = 290;

	dest.x = 55;
	dest.y = 308;
	dest.w = 351;
	dest.h = 290;

	SDL_RenderCopy(renderer, gBackground, 0, 0);
	SDL_RenderCopy(renderer, gPrize, &src, &dest);
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
		if ((level == 4 && length == 3) || (level == 10 && length == 2)) {
			length--;
		}
	}
	return length;
}

void GameOver(){
	isPlaying = false;
	time_delay = BASE_TIME_INTERVAL;
	left_or_right = 1;
	current_level = 0;
	length = 3;
	x_pos = 0;
	underflow_ammt = length - 1;
	memset(array_matrix, 0, sizeof(array_matrix));
}

void GameLoop() {
	SDL_Event event;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN: 
					if (event.key.keysym.sym == SDLK_SPACE && isPlaying) {
						length = GetNewLength(current_level);
						underflow_ammt = length - 1;
						Mix_PlayChannel( -1, sPlace, 0 );
						if (current_level >= GAME_ROWS || length == 0) {
							GameOver();
						}
						current_level++;
						time_delay = time_delay/SPEED_INCREASE;
					}
					if (event.key.keysym.sym == SDLK_1 && credits > 0){
						Mix_PlayChannel( -1, sStart, 0 );
						credits--;
						isPlaying = true;
					}
					if (event.key.keysym.sym == SDLK_5 && credits < 9){
						Mix_PlayChannel( -1, sCredit, 0 );
						credits++;
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
			RenderBackground();
			if (isPlaying)
			{
				Board->UpdateBoard(x_pos, length, current_level);
				Board->RenderBoard();
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

	window = SDL_CreateWindow("Stacker",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          768, 1364,
                                          0);

    if (window == NULL) {
		std::cout << "Unable to set video mode: " << SDL_GetError() << std::endl;
		return false;
    }

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED );

    gBlock = LoadTexture("square.bmp", renderer);
    gBackground = LoadTexture("background.bmp", renderer);
    gPrize = LoadTexture("prize.bmp", renderer);

    if (gBlock == NULL) {
		std::cout << "Unable to load bitmap block" << std::endl;
		return false;
    }
	if (gBackground == NULL) {
		std::cout << "Unable to load bitmap background" << std::endl;
		return false;
    }
	if (gPrize == NULL) {
		std::cout << "Unable to load bitmap prize" << std::endl;
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

	return true;
}

void CloseGame(){
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(gBlock);
	SDL_DestroyTexture(gBackground);
	SDL_DestroyTexture(gPrize);

	window = nullptr;
	gBlock = nullptr;
	gBackground = nullptr;
	gPrize = nullptr;

	Mix_FreeChunk(sStart);
	Mix_FreeChunk(sPlace);
	Mix_FreeChunk(sCredit);

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
