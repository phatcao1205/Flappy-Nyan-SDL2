// menu.cpp
#include "menu.h"
#include "constants.h"

// Hàm khởi tạo Menu
Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), currentFrame(0), frameDelay(500), frameTimer(0) {
    // Tải 2 texture cho animation: message1.png và message2.png
    const char* messageFiles[2] = {
        "assets/message1.png",
        "assets/message2.png"
    };

    for (int i = 0; i < 2; i++) {
        SDL_Surface* surface = IMG_Load(messageFiles[i]);
        messageTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    // Đặt vị trí và kích thước của message (căn giữa màn hình)
    messageRect.w = 722/2;  
    messageRect.h = 969/2;
    messageRect.x = (SCREEN_WIDTH - messageRect.w) / 2;
    messageRect.y = (SCREEN_HEIGHT - messageRect.h) / 2;
}

// Hàm hủy, giải phóng tài nguyên
Menu::~Menu() {
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(messageTextures[i]);
    }
}

// Vẽ menu với animation
void Menu::render() {
    // Cập nhật animation
    frameTimer += 16;  // Giả sử mỗi frame của game là 16ms (60 FPS)
    if (frameTimer >= frameDelay) {
        frameTimer = 0;
        currentFrame = (currentFrame + 1) % 2;  // Chuyển đổi giữa 2 frame
    }

    // Vẽ frame hiện tại
    if (messageTextures[currentFrame]) {
        SDL_RenderCopy(renderer, messageTextures[currentFrame], nullptr, &messageRect);
    }
}