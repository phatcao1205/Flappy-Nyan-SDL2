// background.h
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Lớp Background quản lý nền và mặt đất
class Background {
public:
    // Hàm khởi tạo Background
    Background(SDL_Renderer* renderer);
    // Hàm hủy Background
    ~Background();
    // Cập nhật vị trí của background và base
    void update();
    // Vẽ background và base
    void render();
    // Lấy chiều cao của base
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

#endif // BACKGROUND_H