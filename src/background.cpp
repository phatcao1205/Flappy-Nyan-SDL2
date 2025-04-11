#include "background.h"
#include "constants.h"

Background::Background(SDL_Renderer* renderer) : renderer(renderer), backgroundTexture(nullptr), baseTexture(nullptr), backgroundX(0), baseX(0) {
    SDL_Surface* bgSurface = IMG_Load("assets/background-night.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    backgroundWidth = bgSurface->w;
    SDL_FreeSurface(bgSurface);

    SDL_Surface* baseSurface = IMG_Load("assets/base.png");
    baseTexture = SDL_CreateTextureFromSurface(renderer, baseSurface);
    baseWidth = baseSurface->w;
    baseHeight = baseSurface->h;
    SDL_FreeSurface(baseSurface);
}

Background::~Background() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(baseTexture);
}

void Background::update() {
    backgroundX -= BACKGROUND_SPEED;
    if (backgroundX <= -backgroundWidth) {
        backgroundX = 0;
    }

    baseX -= BASE_SPEED;
    if (baseX <= -baseWidth) {
        baseX = 0;
    }
}

void Background::render() {
    for (int x = backgroundX; x < SCREEN_WIDTH; x += backgroundWidth) {
        SDL_Rect bgDst = {x, 0, backgroundWidth, SCREEN_HEIGHT - baseHeight};
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgDst);
    }

    for (int x = baseX; x < SCREEN_WIDTH; x += baseWidth) {
        SDL_Rect baseDst = {x, SCREEN_HEIGHT - baseHeight, baseWidth, baseHeight};
        SDL_RenderCopy(renderer, baseTexture, nullptr, &baseDst);
    }
}

int Background::getBaseHeight() {
    return baseHeight;
}