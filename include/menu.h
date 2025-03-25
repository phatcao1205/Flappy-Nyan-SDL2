// menu.h
#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Lớp Menu quản lý menu bắt đầu của trò chơi
class Menu {
public:
    Menu(SDL_Renderer* renderer);  // Hàm khởi tạo
    ~Menu();                       // Hàm hủy
    void render();                 // Vẽ menu

private:
    SDL_Renderer* renderer;
    SDL_Texture* messageTexture;   // Texture của message.png
    SDL_Rect messageRect;          // Vị trí và kích thước của message.png
};

#endif