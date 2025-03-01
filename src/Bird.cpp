#include "Bird.h"

Bird createBird() {
    Bird bird;
    bird.rect.x = SCREEN_WIDTH / 4;
    bird.rect.y = SCREEN_HEIGHT / 2;
    bird.rect.w = 32;
    bird.rect.h = 32;
    bird.y_velocity = 0;
    return bird;
}

void drawBird(SDL_Renderer* renderer, const Bird& bird) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF); // Màu vàng
    SDL_RenderFillRect(renderer, &bird.rect);
}