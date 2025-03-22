// background.cpp
#include "background.h"
#include "constants.h"

// Hàm khởi tạo Background
Background::Background(SDL_Renderer* renderer) : renderer(renderer), backgroundTexture(nullptr), baseTexture(nullptr), baseHeight(0) {
    // Tải background-day.png
    SDL_Surface* bgSurface = IMG_Load("assets/background-day.png");
    if (!bgSurface) {
        SDL_Log("Không thể tải background-day.png: %s", IMG_GetError());
    } else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
        if (!backgroundTexture) {
            SDL_Log("Không thể tạo texture background: %s", SDL_GetError());
        }
    }

    // Tải base.png
    SDL_Surface* baseSurface = IMG_Load("assets/base.png");
    if (!baseSurface) {
        SDL_Log("Không thể tải base.png: %s", IMG_GetError());
    } else {
        baseTexture = SDL_CreateTextureFromSurface(renderer, baseSurface);
        baseHeight = baseSurface->h;  // Lấy chiều cao của base
        SDL_FreeSurface(baseSurface);
        if (!baseTexture) {
            SDL_Log("Không thể tạo texture base: %s", SDL_GetError());
        }
    }
}

// Hàm hủy, giải phóng tài nguyên
Background::~Background() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (baseTexture) SDL_DestroyTexture(baseTexture);
}

// Lấy chiều cao của base
int Background::getBaseHeight() const {
    return baseHeight;
}

// Vẽ nền và mặt đất
void Background::render() {
    // Vẽ background-day.png lặp lại
    if (backgroundTexture) {
        int bgWidth, bgHeight;
        SDL_QueryTexture(backgroundTexture, nullptr, nullptr, &bgWidth, &bgHeight);

        // Lặp lại background để lấp đầy màn hình
        for (int x = 0; x < SCREEN_WIDTH; x += bgWidth) {
            SDL_Rect dstRect = {x, 0, bgWidth, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dstRect);
        }
    }

    // Vẽ base.png ở dưới màn hình
    if (baseTexture) {
        int baseWidth;
        SDL_QueryTexture(baseTexture, nullptr, nullptr, &baseWidth, &baseHeight);

        // Lặp lại base để lấp đầy chiều ngang
        for (int x = 0; x < SCREEN_WIDTH; x += baseWidth) {
            SDL_Rect dstRect = {x, SCREEN_HEIGHT - baseHeight, baseWidth, baseHeight};
            SDL_RenderCopy(renderer, baseTexture, nullptr, &dstRect);
        }
    }
}