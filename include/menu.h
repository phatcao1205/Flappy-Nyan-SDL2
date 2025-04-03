// menu.h
#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Lớp Menu quản lý hiển thị và animation của màn hình menu
class Menu {
public:
    // Hàm khởi tạo Menu
    Menu(SDL_Renderer* renderer);
    // Hàm hủy Menu
    ~Menu();
    // Vẽ menu lên renderer
    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* messageTextures[2];
    SDL_Rect messageRect;
    int currentFrame;
    int frameDelay;
    int frameTimer;
};

#endif // MENU_H