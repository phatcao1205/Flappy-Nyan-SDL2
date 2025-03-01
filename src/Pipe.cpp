#include "Pipe.h"
#include <cstdlib> // Cần thiết cho rand()

Pipe createPipe(int x) {
    Pipe pipe;
    int gapHeight = 150;
    int gapPosition = rand() % (SCREEN_HEIGHT - gapHeight - 50) + 50; // Giữ cho gap không quá gần trên/dưới

    pipe.top.x = x;
    pipe.top.y = 0;
    pipe.top.w = 50;
    pipe.top.h = gapPosition;

    pipe.bottom.x = x;
    pipe.bottom.y = gapPosition + gapHeight;
    pipe.bottom.w = 50;
    pipe.bottom.h = SCREEN_HEIGHT - (gapPosition + gapHeight);

    return pipe;
}

void drawPipe(SDL_Renderer* renderer, const Pipe& pipe) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // Màu xanh lá cây
    SDL_RenderFillRect(renderer, &pipe.top);
    SDL_RenderFillRect(renderer, &pipe.bottom);
}

bool checkCollision(const Bird& bird, const Pipe& pipe) {
    if (SDL_HasIntersection(&bird.rect, &pipe.top) || SDL_HasIntersection(&bird.rect, &pipe.bottom)) {
        return true;
    }
    return false;
}