// menu.h
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
    SDL_Texture* messageTextures[2];  // Texture cho 2 frame: message1.png và message2.png
    SDL_Rect messageRect;             // Vị trí và kích thước của message
    int currentFrame;                 // Frame hiện tại của animation
    int frameDelay;                   // Thời gian giữa các frame (ms)
    int frameTimer;                   // Bộ đếm thời gian để chuyển frame
};

#endif