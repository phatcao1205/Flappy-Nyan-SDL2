#include "pipe.h"
#include "constants.h"

PipeManager::PipeManager(SDL_Renderer* renderer) : pipeTexture(nullptr), pipeWidth(PIPE_WIDTH) {
    
    SDL_Surface* surface = IMG_Load("assets/pipe-green.png");
    if (!surface) {
        SDL_Log("Không thể tải pipe-green.png: %s", IMG_GetError());
    } else {
        pipeTexture = SDL_CreateTextureFromSurface(renderer, surface);
        pipeWidth = surface->w;  
        SDL_FreeSurface(surface);
        if (!pipeTexture) {
            SDL_Log("Không thể tạo texture pipe: %s", SDL_GetError());
        }
    }
}

PipeManager::~PipeManager() {
    if (pipeTexture) {
        SDL_DestroyTexture(pipeTexture);
    }
}

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
int PipeManager::getPipeWidth() const {
    return pipeWidth;
}