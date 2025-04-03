// bird.cpp
#include "bird.h"
#include "constants.h"
#include <cmath> // Để sử dụng sin()

// Hàm khởi tạo BirdManager, tải các texture animation của mèo
BirdManager::BirdManager(SDL_Renderer* renderer) : renderer(renderer), currentFrame(0), frameDelay(70), frameTimer(0), hoverTimer(0) {
    const char* birdFiles[12] = {
        "assets/cat1.png", "assets/cat2.png", "assets/cat3.png", "assets/cat4.png",
        "assets/cat5.png", "assets/cat6.png", "assets/cat7.png", "assets/cat8.png",
        "assets/cat9.png", "assets/cat10.png", "assets/cat11.png", "assets/cat12.png"
    };

    for (int i = 0; i < 12; i++) {
        SDL_Surface* surface = IMG_Load(birdFiles[i]);
        birdTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

// Hàm hủy, giải phóng tài nguyên textures của mèo
BirdManager::~BirdManager() {
    for (int i = 0; i < 12; i++) {
        SDL_DestroyTexture(birdTextures[i]);
    }
}

// Cập nhật trạng thái chim (vị trí, vận tốc, animation và góc xoay) tùy theo GameState
void BirdManager::updateBird(Bird& bird, GameState gameState) {
    if (gameState == MENU || gameState == SETTING) {
        // Hiệu ứng lơ lửng ở menu/setting
        hoverTimer += 0.1f;
        float hoverAmplitude = 10.0f;
        float hoverOffset = sin(hoverTimer) * hoverAmplitude;
        bird.rect.y = (SCREEN_HEIGHT / 2) + static_cast<int>(hoverOffset);
        bird.velocity = 0;
        bird.angle = 0;
    } else if (gameState == PLAYING) {
        // Áp dụng trọng lực và cập nhật góc xoay khi chơi
        bird.velocity += GRAVITY;
        bird.rect.y += static_cast<int>(bird.velocity);

        const double MAX_UP_ANGLE = -45.0;
        const double MAX_DOWN_ANGLE = 90.0;
        const double ANGLE_PER_VELOCITY = 2.5;
        const double SMOOTHING_FACTOR = 0.1;

        double targetAngle = bird.velocity * ANGLE_PER_VELOCITY;
        targetAngle = std::max(MAX_UP_ANGLE, std::min(targetAngle, MAX_DOWN_ANGLE)); // Clamp angle
        bird.angle += (targetAngle - bird.angle) * SMOOTHING_FACTOR; // Smooth rotation
    }

    // Cập nhật frame animation
    frameTimer += 16; // Giả sử 60 FPS
    if (frameTimer >= frameDelay) {
        frameTimer = 0;
        currentFrame = (currentFrame + 1) % 12;
    }
}

// Vẽ frame hiện tại của chim lên renderer với góc xoay phù hợp
void BirdManager::render(SDL_Renderer* renderer, const Bird& bird) {
    if (birdTextures[currentFrame]) {
        SDL_RenderCopyEx(renderer, birdTextures[currentFrame], nullptr, &bird.rect, bird.angle, nullptr, SDL_FLIP_NONE);
    }
}