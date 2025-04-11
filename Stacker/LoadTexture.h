#pragma once

#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

SDL_Texture* LoadTexture(std::string filePath, SDL_Renderer* renderTarget);
bool CheckTexture(SDL_Texture* texture, std::string path, SDL_Window* window);