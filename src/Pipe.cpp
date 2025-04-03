// pipe.cpp
#include "pipe.h"
#include "constants.h"

// Hàm khởi tạo PipeManager
PipeManager::PipeManager(SDL_Renderer* renderer) : pipeTexture(nullptr), pipeWidth(PIPE_WIDTH) {
    // Tải pipe-green.png
    SDL_Surface* surface = IMG_Load("assets/pipe-green.png");
    if (!surface) {
        SDL_Log("Không thể tải pipe-green.png: %s", IMG_GetError());
    } else {
        pipeTexture = SDL_CreateTextureFromSurface(renderer, surface);
        pipeWidth = surface->w;  // Lấy chiều rộng thực tế của ống từ hình ảnh
        SDL_FreeSurface(surface);
        if (!pipeTexture) {
            SDL_Log("Không thể tạo texture pipe: %s", SDL_GetError());
        }
    }
}

// Hàm hủy, giải phóng tài nguyên
PipeManager::~PipeManager() {
    if (pipeTexture) {
        SDL_DestroyTexture(pipeTexture);
    }
}

// Hàm kiểm tra va chạm giữa chim và ống
bool PipeManager::checkCollision(int bx, int by, int bw, int bh, int px, int ph) {
    return (bx < px + pipeWidth && bx + bw > px &&
            (by < ph || by + bh > ph + PIPE_GAP));
}

// Vẽ một cặp ống (trên và dưới)
void PipeManager::render(SDL_Renderer* renderer, const Pipe& pipe, int baseHeight) {
    if (!pipeTexture) return;

    int pipeHeight;
    SDL_QueryTexture(pipeTexture, nullptr, nullptr, nullptr, &pipeHeight);

    int topPipeHeight = pipe.height;
    int y = topPipeHeight;
    while (y > 0) {
        int drawHeight = std::min(pipeHeight, y);
        SDL_Rect topPipeSrc = {0, 0, pipeWidth, drawHeight};
        SDL_Rect topPipeDst = {pipe.x, y - drawHeight, pipeWidth, drawHeight};
        SDL_RenderCopyEx(renderer, pipeTexture, &topPipeSrc, &topPipeDst, 180.0, nullptr, SDL_FLIP_NONE);
        y -= drawHeight;
    }

   
    int bottomPipeY = SCREEN_HEIGHT - baseHeight;
    int bottomPipeHeight = bottomPipeY - (pipe.height + PIPE_GAP);

    if (bottomPipeHeight < MIN_BOTTOM_PIPE_HEIGHT) {
        bottomPipeHeight = MIN_BOTTOM_PIPE_HEIGHT;
    }
    y = pipe.height + PIPE_GAP;
    int totalHeightDrawn = 0;
    while (totalHeightDrawn < bottomPipeHeight) {
        int remainingHeight = bottomPipeHeight - totalHeightDrawn;
        int drawHeight = std::min(pipeHeight, remainingHeight);
        SDL_Rect bottomPipeSrc = {0, 0, pipeWidth, drawHeight};
        SDL_Rect bottomPipeDst = {pipe.x, y + totalHeightDrawn, pipeWidth, drawHeight};
        SDL_RenderCopy(renderer, pipeTexture, &bottomPipeSrc, &bottomPipeDst);
        totalHeightDrawn += drawHeight;
    }
}