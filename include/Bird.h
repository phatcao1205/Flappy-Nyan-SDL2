// bird.h
#ifndef BIRD_H
#define BIRD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "gamestate.h"

// Cấu trúc Bird đại diện cho trạng thái của mèo
struct Bird {
    SDL_Rect rect;
    float velocity;
    double angle;
};

// Lớp BirdManager quản lý texture, animation và logic của mèo
class BirdManager {
public:
    // Hàm khởi tạo BirdManager
    BirdManager(SDL_Renderer* renderer);
    // Hàm hủy BirdManager
    ~BirdManager();
    // Cập nhật trạng thái mèo (vị trí, vận tốc, animation, góc xoay) dựa trên GameState
    void updateBird(Bird& bird, GameState gameState);
    // Vẽ mèo lên renderer
    void render(SDL_Renderer* renderer, const Bird& bird);

private:
    SDL_Renderer* renderer;
    SDL_Texture* birdTextures[12];
    int currentFrame;
    int frameDelay;
    int frameTimer;
    float hoverTimer;
};

#endif // BIRD_H