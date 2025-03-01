#ifndef PIPE_H
#define PIPE_H

#include <SDL2/SDL.h>
#include "Constants.h"
#include "Bird.h"

struct Pipe {
    SDL_Rect top;
    SDL_Rect bottom;
};

Pipe createPipe(int x);
void drawPipe(SDL_Renderer* renderer, const Pipe& pipe);
bool checkCollision(const Bird& bird, const Pipe& pipe);

#endif