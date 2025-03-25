// background.cpp
#include "background.h"
#include "constants.h"

// Hàm khởi tạo Background
Background::Background(SDL_Renderer* renderer) : renderer(renderer), backgroundTexture(nullptr), baseTexture(nullptr), backgroundX(0), baseX(0) {
    // Tải background.png
    SDL_Surface* bgSurface = IMG_Load("assets/background-night.png");
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        backgroundWidth = bgSurface->w;  // Lấy chiều rộng của background
        SDL_FreeSurface(bgSurface);

    // Tải base.png
    SDL_Surface* baseSurface = IMG_Load("assets/base.png");
        baseTexture = SDL_CreateTextureFromSurface(renderer, baseSurface);
        baseWidth = baseSurface->w;      // Lấy chiều rộng của base
        baseHeight = baseSurface->h;     // Lấy chiều cao của base
        SDL_FreeSurface(baseSurface);
}

// Hàm hủy, giải phóng tài nguyên
Background::~Background() {
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyTexture(baseTexture);
}

// Cập nhật vị trí của background và base
void Background::update() {
    // Di chuyển background sang trái
    backgroundX -= BACKGROUND_SPEED;
    if (backgroundX <= -backgroundWidth) {
        backgroundX = 0;  // Đặt lại vị trí khi background ra khỏi màn hình
    }

    // Di chuyển base sang trái
    baseX -= BASE_SPEED;
    if (baseX <= -baseWidth) {
        baseX = 0;  // Đặt lại vị trí khi base ra khỏi màn hình
    }
}

// Vẽ background và base
void Background::render() {
    // Vẽ background (lặp lại để lấp đầy màn hình)
    for (int x = backgroundX; x < SCREEN_WIDTH; x += backgroundWidth) {
        SDL_Rect bgDst = {x, 0, backgroundWidth, SCREEN_HEIGHT - baseHeight};
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgDst);
    }

    // Vẽ base (lặp lại để lấp đầy màn hình)
    for (int x = baseX; x < SCREEN_WIDTH; x += baseWidth) {
        SDL_Rect baseDst = {x, SCREEN_HEIGHT - baseHeight, baseWidth, baseHeight};
        SDL_RenderCopy(renderer, baseTexture, nullptr, &baseDst);
    }
}

// Lấy chiều cao của base
int Background::getBaseHeight() {
    return baseHeight;
}