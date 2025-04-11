#pragma once
#include <SDL.h>

#include "GameConstants.h"

class GameBoard
{

public:
	GameBoard();

	void RenderBoard(SDL_Renderer* renderer, SDL_Texture* gBlock, int array_matrix[GAME_ROWS][GAME_COLUMNS]);
	void UpdateBoard(uint8_t underflow_ammt, int array_matrix[GAME_ROWS][GAME_COLUMNS], int x_pos, int length, int level);
	void DeleteBoard();
};