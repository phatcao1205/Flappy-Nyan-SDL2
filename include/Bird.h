#ifndef BIRD_H
#define BIRD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "gamestate.h"

struct Bird {
    SDL_Rect rect;
    float velocity;
    double angle;
};

class BirdManager {
public:
    BirdManager(SDL_Renderer* renderer);
    ~BirdManager();
    void updateBird(Bird& bird, GameState gameState);
    void render(SDL_Renderer* renderer, const Bird& bird);

private:
    SDL_Renderer* renderer;
    SDL_Texture* birdTextures[12];
    int currentFrame;
    int frameDelay;
    int frameTimer;
    float hoverTimer;
};

#endif