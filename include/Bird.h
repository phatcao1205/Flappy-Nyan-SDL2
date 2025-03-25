// bird.h
#ifndef BIRD_H
#define BIRD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "gamestate.h"

// Cấu trúc Bird đại diện cho mèo (trước đây là chim)
struct Bird {
    SDL_Rect rect;  // Vị trí và kích thước của mèo
    float velocity; // Vận tốc của mèo
    double angle;   // Góc xoay của mèo (tính bằng độ)
};

// Lớp BirdManager quản lý texture và animation của mèo
class BirdManager {
public:
    BirdManager(SDL_Renderer* renderer);  // Hàm khởi tạo
    ~BirdManager();                       // Hàm hủy
    void updateBird(Bird& bird, GameState gameState);  // Cập nhật trạng thái mèo
    void render(SDL_Renderer* renderer, const Bird& bird);  // Vẽ mèo

private:
    SDL_Renderer* renderer;
    SDL_Texture* birdTextures[12];  // Texture cho 12 trạng thái animation (cat1.png đến cat12.png)
    int currentFrame;               // Frame hiện tại của animation
    int frameDelay;                 // Thời gian giữa các frame (ms), đặt là 70ms
    int frameTimer;                 // Bộ đếm thời gian để chuyển frame
    float hoverTimer;               // Bộ đếm thời gian cho hiệu ứng lơ lửng
};

#endif