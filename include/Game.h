#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_image.h>
#include "Bird.h"
#include "Pipe.h"
#include "Constants.h"

bool init(SDL_Window** window, SDL_Renderer** renderer);
void close(SDL_Window* window, SDL_Renderer* renderer);

#endif