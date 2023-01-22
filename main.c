#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define BASE_TIME_INTERVAL 80 /* in microseconds */
#define SPEED_INCREASE 1.03
#define GAME_ROWS 15
#define GAME_COLUMNS 7

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

int array_matrix[GAME_ROWS][GAME_COLUMNS];

static SDL_Window *window;
static SDL_Surface *screen;
static SDL_Surface *block;
static SDL_Surface *background;
static SDL_Surface *prize;

/* Use the array_matrix to  indicate where to blit red sqares on the board.
 * Also do some arithmetic to get them to print in the proper place, given
 * the size of each block is 65 */
void print_board() {
	int i, j;
    SDL_Rect src, dest;

	for (i = 0; i < 15; i++) {
		for (j = 0; j < 7 ; j++) {
			if (array_matrix[i][j] == 1) {
				src.x = 0;
				src.y = 0;
				src.w = 65; 
				src.h = 65;

				dest.x = j * 67 + 2;
				dest.y = i * 67 + 2;
				dest.w = 65;
				dest.h = 65;

				SDL_BlitSurface(block, &src, screen, &dest);
			}
		}
	}
}

void print_background(){
    SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = 351; 
	src.h = 290;

	dest.x = 55;
	dest.y = 308;
	dest.w = 351;
	dest.h = 290;

	SDL_BlitSurface(background, 0, screen, 0);
	SDL_BlitSurface(prize, &src, screen, &dest);
}

/* Level is the active row, with level 1 being the bottom. 
 * x_pos is the current position of the active blocks on the horizontal level. 
   x_pos values:
		0 = left side
		2 = center
		4 = right side
   underflow_ammt is the number of columns that go off the side of the screen.
*/
void update_board(int x_pos, int length, int level) {
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
int get_new_length(int level) {
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

void game_loop() {
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
						length = get_new_length(current_level);
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
			SDL_FillRect(screen, NULL, 0x000000);
			if (x_pos >= GAME_COLUMNS + (underflow_ammt - 1))
				left_or_right = -1;
			if (x_pos <= 0) 
				left_or_right = 1;
			update_board(x_pos, length, current_level);
			print_background();
			print_board();
			x_pos = x_pos + left_or_right;
			SDL_Delay(time_delay);
			SDL_UpdateWindowSurface(window);
		}
	}
}

int main(int argc, char *argv[])
{
    /* Initialize SDL's video system and check for errors. */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
    }

    /* Make sure SDL_Quit gets called when the program exits! */
    atexit(SDL_Quit);

	window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          768, 1364,
                                          0);

    if (window == NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
    }

	screen = SDL_GetWindowSurface(window);

    /* Load the bitmap file. SDL_LoadBMP returns a pointer to a
       new surface containing the loaded image. */
    block = SDL_LoadBMP("square.bmp");
    background = SDL_LoadBMP("background.bmp");
    prize = SDL_LoadBMP("prize.bmp");
    if (block == NULL) {
		printf("Unable to load bitmap.\n");
		return 1;
    }
	if (background == NULL) {
		printf("Unable to load bitmap.\n");
		return 1;
    }
	if (prize == NULL) {
		printf("Unable to load bitmap prize.\n");
		return 1;
    }
	game_loop();

    /* Free the memory that was allocated to the bitmap. */
    SDL_FreeSurface(block);
    SDL_FreeSurface(background);
    SDL_FreeSurface(prize);

    return 0;
}
