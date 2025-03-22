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
    void update();                       // Cập nhật vị trí của background và base
    void render();                       // Vẽ background và base
    int getBaseHeight();                 // Lấy chiều cao của base

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;  // Texture của background
    SDL_Texture* baseTexture;        // Texture của base (mặt đất)
    int backgroundWidth;             // Chiều rộng của background
    int baseWidth;                   // Chiều rộng của base
    int baseHeight;                  // Chiều cao của base
    int backgroundX;                 // Vị trí x của background
    int baseX;                       // Vị trí x của base
};

#endif