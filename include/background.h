#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Background {
public:
    Background(SDL_Renderer* renderer);
    ~Background();
    void update();
    void render();
    int getBaseHeight();
    
private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* baseTexture;
    int backgroundWidth;
    int baseWidth;
    int baseHeight;
    int backgroundX;
    int baseX;
};

#endif