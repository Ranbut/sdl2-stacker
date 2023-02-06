#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#define BASE_TIME_INTERVAL 150 /* in microseconds */
#define SPEED_INCREASE 1.03
#define GAME_ROWS 26
#define GAME_COLUMNS 5

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

int array_matrix[GAME_ROWS][GAME_COLUMNS];

//Images
SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *block;
SDL_Texture *background;
SDL_Texture *prize;

//Font
TTF_Font *fontCredit = TTF_OpenFont("arial.ttf", 40);
SDL_Color colorCredit = { 255, 255, 255, 255 };
SDL_Surface *surfaceCredit = TTF_RenderText_Solid(fontCredit, "Aqui está o credito", colorCredit);
SDL_Texture *textCredit = SDL_CreateTextureFromSurface(renderer, surfaceCredit);

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

/* Use the array_matrix to  indicate where to blit red sqares on the board.
 * Also do some arithmetic to get them to print in the proper place, given
 * the size of each block is 65 */
void RenderBoard() {
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

				SDL_RenderCopy(renderer, block, &src, &dest);
			}
		}
	}
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

	SDL_RenderCopy(renderer, background, 0, 0);
	SDL_RenderCopy(renderer, prize, &src, &dest);
}

void RenderCredit(){
	SDL_Rect textRect;
	textRect.x = textRect.y = 0;

	SDL_QueryTexture(textCredit, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textCredit, NULL, &textRect);
}

/* Level is the active row, with level 1 being the bottom. 
 * x_pos is the current position of the active blocks on the horizontal level. 
   x_pos values:
		0 = left side
		2 = center
		4 = right side
   underflow_ammt is the number of columns that go off the side of the screen.
*/
void UpdateBoard(int x_pos, int length, int level) {
	int underflow_ammt = length - 1;
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

void GameLoop() {
	int time_delay = BASE_TIME_INTERVAL;
	int left_or_right = 1;
	int current_level = 1;
	int length = 3;
	int x_pos = 0;
	int quit = 0;
	int underflow_ammt = length - 1;
	SDL_Event event;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN: 
					if (event.key.keysym.sym == SDLK_SPACE) {
						length = GetNewLength(current_level);
						underflow_ammt = length - 1;
						if (current_level >= 15 || length == 0) {
							quit = 1;
						}
						current_level++;
						time_delay = time_delay/SPEED_INCREASE;
					}
					break;
				case SDL_QUIT:
					quit = 1;
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
			UpdateBoard(x_pos, length, current_level);
			RenderBackground();
			RenderBoard();
			RenderCredit();
			SDL_RenderPresent(renderer);
			x_pos = x_pos + left_or_right;
			SDL_Delay(time_delay);
			SDL_UpdateWindowSurface(window);
		}

		SDL_RenderClear(renderer);
	}
}

bool InitGame(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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

	window = SDL_CreateWindow("Stacker",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          768, 1364,
                                          0);

    if (window == NULL) {
		std::cout << "Unable to set video mode: " << SDL_GetError() << std::endl;
		return false;
    }

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    block = LoadTexture("square.bmp", renderer);
    background = LoadTexture("background.bmp", renderer);
    prize = LoadTexture("prize.bmp", renderer);

    if (block == NULL) {
		std::cout << "Unable to load bitmap block" << std::endl;
		return false;
    }
	if (background == NULL) {
		std::cout << "Unable to load bitmap background" << std::endl;
		return false;
    }
	if (prize == NULL) {
		std::cout << "Unable to load bitmap prize" << std::endl;
		return false;
    }

	return true;
}

void CloseGame(){
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(block);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(prize);

	window = nullptr;
	block = nullptr;
	background = nullptr;
	prize = nullptr;

	SDL_DestroyTexture(textCredit);

	textCredit = nullptr;

	TTF_Quit();
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
