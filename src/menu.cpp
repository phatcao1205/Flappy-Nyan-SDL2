#include "menu.h"
#include "constants.h"

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), currentFrame(0), frameDelay(500), frameTimer(0) {
    const char* messageFiles[2] = {"assets/message1.png", "assets/message2.png"};
    for (int i = 0; i < 2; i++) {
        SDL_Surface* surface = IMG_Load(messageFiles[i]);
        messageTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    
    messageRect = {(SCREEN_WIDTH - 722/2) / 2, (SCREEN_HEIGHT - 969/2) / 2, 722/2, 969/2};
}

Menu::~Menu() {
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(messageTextures[i]);
    }
}

void Menu::render() {
    frameTimer += 16; 
    if (frameTimer >= frameDelay) {
        frameTimer = 0;
        currentFrame = (currentFrame + 1) % 2;
    }

    if (messageTextures[currentFrame]) {
        SDL_RenderCopy(renderer, messageTextures[currentFrame], nullptr, &messageRect);
    }
}