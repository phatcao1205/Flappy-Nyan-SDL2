// background.h
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Lớp Background quản lý nền và mặt đất
class Background {
public:
    Background(SDL_Renderer* renderer);  // Hàm khởi tạo
    ~Background();                       // Hàm hủy
    void render();                       // Vẽ nền và mặt đất
    int getBaseHeight() const;           // Lấy chiều cao của base

private:
    SDL_Renderer* renderer;              // Renderer để vẽ
    SDL_Texture* backgroundTexture;      // Texture của background-day.png
    SDL_Texture* baseTexture;            // Texture của base.png
    int baseHeight;                      // Chiều cao của base
};

#endif