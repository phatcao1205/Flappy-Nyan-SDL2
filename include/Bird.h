#ifndef BIRD_H
#define BIRD_H

#include <SDL2/SDL.h>
#include "Constants.h"

struct Bird {
    SDL_Rect rect;
    float y_velocity;
};

Bird createBird();
void drawBird(SDL_Renderer* renderer, const Bird& bird);

#endif