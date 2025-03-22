// bird.cpp
#include "bird.h"
#include "constants.h"

// Hàm khởi tạo BirdManager
BirdManager::BirdManager(SDL_Renderer* renderer) : renderer(renderer), currentFrame(0), frameDelay(100), frameTimer(0) {
    // Tải 3 texture cho các trạng thái vỗ cánh
    const char* birdFiles[3] = {
        "assets/yellowbird-upflap.png",
        "assets/yellowbird-midflap.png",
        "assets/yellowbird-downflap.png"
    };

    for (int i = 0; i < 3; i++) {
        SDL_Surface* surface = IMG_Load(birdFiles[i]);
        if (!surface) {
            SDL_Log("Không thể tải %s: %s", birdFiles[i], IMG_GetError());
            birdTextures[i] = nullptr;
            continue;
        }
        birdTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!birdTextures[i]) {
            SDL_Log("Không thể tạo texture từ %s: %s", birdFiles[i], SDL_GetError());
        }
    }
}

// Hàm hủy, giải phóng tài nguyên
BirdManager::~BirdManager() {
    for (int i = 0; i < 3; i++) {
        if (birdTextures[i]) {
            SDL_DestroyTexture(birdTextures[i]);
        }
    }
}

// Cập nhật trạng thái chim (vị trí, vận tốc, animation và góc xoay)
void BirdManager::updateBird(Bird& bird) {
    bird.velocity += GRAVITY;  // Áp dụng trọng lực
    bird.rect.y += static_cast<int>(bird.velocity);  // Cập nhật vị trí y

    // Tính góc xoay mục tiêu dựa trên vận tốc
    const double MAX_UP_ANGLE = -45.0;  // Góc tối đa khi nghiêng lên
    const double MAX_DOWN_ANGLE = 90.0; // Góc tối đa khi nghiêng xuống
    const double ANGLE_PER_VELOCITY = 4.5;  // Giảm hệ số để xoay chậm hơn (trước là 5.0)
    const double SMOOTHING_FACTOR = 0.1;    // Hệ số làm mượt (0.0 đến 1.0, càng thấp càng mượt)

    // Tính góc xoay mục tiêu
    double targetAngle = bird.velocity * ANGLE_PER_VELOCITY;
    if (targetAngle < MAX_UP_ANGLE) {
        targetAngle = MAX_UP_ANGLE;
    }
    if (targetAngle > MAX_DOWN_ANGLE) {
        targetAngle = MAX_DOWN_ANGLE;
    }

    // Làm mượt góc xoay: tiến gần đến targetAngle một cách từ từ
    bird.angle += (targetAngle - bird.angle) * SMOOTHING_FACTOR;

    // Cập nhật animation
    frameTimer += 16;  // Giả sử mỗi frame của game là 16ms (60 FPS)
    if (frameTimer >= frameDelay) {
        frameTimer = 0;
        currentFrame = (currentFrame + 1) % 3;  // Chuyển đổi giữa 3 frame
    }
}

// Vẽ chim với góc xoay
void BirdManager::render(SDL_Renderer* renderer, const Bird& bird) {
    if (birdTextures[currentFrame]) {
        // Sử dụng SDL_RenderCopyEx để vẽ chim với góc xoay
        SDL_RenderCopyEx(renderer, birdTextures[currentFrame], nullptr, &bird.rect, bird.angle, nullptr, SDL_FLIP_NONE);
    }
}