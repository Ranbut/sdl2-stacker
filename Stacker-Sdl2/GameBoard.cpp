#include "GameBoard.h"

GameBoard::GameBoard(){}

void GameBoard::RenderBoard(SDL_Renderer* renderer, SDL_Texture* gBlock, int array_matrix[GAME_ROWS][GAME_COLUMNS]) {
	int i, j;
	SDL_Rect src, dest;

	for (i = 0; i < GAME_ROWS; i++) {
		for (j = 0; j < GAME_COLUMNS; j++) {
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

void GameBoard::UpdateBoard(uint8_t underflow_ammt, int array_matrix[GAME_ROWS][GAME_COLUMNS], int x_pos, int length, int level) {
	int j;

	if (x_pos < underflow_ammt)
		length = length - (underflow_ammt - x_pos);

	x_pos = MAX(0, x_pos - underflow_ammt);

	for (j = 0; j < GAME_COLUMNS; j++)
		array_matrix[GAME_ROWS - level][j] = 0;

	for (j = x_pos; j < x_pos + length; j++) {
		array_matrix[GAME_ROWS - level][MIN(j, GAME_COLUMNS - 1)] = 1;
	}
}

void GameBoard::DeleteBoard()
{

}
