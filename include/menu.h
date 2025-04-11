#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();
    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* messageTextures[2];
    SDL_Rect messageRect;
    int currentFrame;
    int frameDelay;
    int frameTimer;
};

#endif