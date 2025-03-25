// menu.cpp
#include "menu.h"
#include "constants.h"

// Hàm khởi tạo Menu
Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), messageTexture(nullptr) {
    // Tải message.png
    SDL_Surface* surface = IMG_Load("assets/message.png");
    if (!surface) {
        SDL_Log("Không thể tải message.png: %s", IMG_GetError());
    } else {
        messageTexture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!messageTexture) {
            SDL_Log("Không thể tạo texture message: %s", SDL_GetError());
        }

        // Đặt vị trí và kích thước của message.png (căn giữa màn hình)
        int width = surface->w;
        int height = surface->h;
        messageRect = { (SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2, width, height };
        SDL_FreeSurface(surface);
    }
}

// Hàm hủy, giải phóng tài nguyên
Menu::~Menu() {
    if (messageTexture) {
        SDL_DestroyTexture(messageTexture);
    }
}

// Vẽ menu
void Menu::render() {
    if (messageTexture) {
        SDL_RenderCopy(renderer, messageTexture, nullptr, &messageRect);
    }
}