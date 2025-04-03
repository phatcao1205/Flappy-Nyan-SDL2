// menu.cpp
#include "menu.h"
#include "constants.h"

// Hàm khởi tạo Menu, tải textures và thiết lập vị trí
Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), currentFrame(0), frameDelay(500), frameTimer(0) {
    const char* messageFiles[2] = {"assets/message1.png", "assets/message2.png"};
    for (int i = 0; i < 2; i++) {
        SDL_Surface* surface = IMG_Load(messageFiles[i]);
        messageTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    // Giả sử kích thước gốc là 722x969, chia đôi để hiển thị
    messageRect = {(SCREEN_WIDTH - 722/2) / 2, (SCREEN_HEIGHT - 969/2) / 2, 722/2, 969/2};
}

// Hàm hủy, giải phóng textures của menu
Menu::~Menu() {
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(messageTextures[i]);
    }
}

// Vẽ menu, cập nhật animation giữa các frame
void Menu::render() {
    frameTimer += 16; // Giả sử 60 FPS
    if (frameTimer >= frameDelay) {
        frameTimer = 0;
        currentFrame = (currentFrame + 1) % 2;
    }

    if (messageTextures[currentFrame]) {
        SDL_RenderCopy(renderer, messageTextures[currentFrame], nullptr, &messageRect);
    }
}