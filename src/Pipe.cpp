#include "pipe.h"
#include "constants.h"

PipeManager::PipeManager(SDL_Renderer* renderer) : pipeTexture(nullptr), pipeWidth(PIPE_WIDTH) { 
    SDL_Surface* surface = IMG_Load("assets/pipe-green.png");
        pipeTexture = SDL_CreateTextureFromSurface(renderer, surface);
        pipeWidth = surface->w;  
        SDL_FreeSurface(surface);
}

PipeManager::~PipeManager() {
    if (pipeTexture) {
        SDL_DestroyTexture(pipeTexture);
    }
}

void PipeManager::render(SDL_Renderer* renderer, const Pipe& pipe, int baseHeight) {
    if (!pipeTexture) return;

    int pipeTextureHeight;
    SDL_QueryTexture(pipeTexture, nullptr, nullptr, nullptr, &pipeTextureHeight);

    int y=pipe.height;

    while (y > 0) {
        int drawHeight = std::min(pipeTextureHeight, y);
        SDL_Rect topPipeStart = {0, 0, pipeWidth, drawHeight};
        SDL_Rect topPipeEnd = {pipe.x, y - drawHeight, pipeWidth, drawHeight};
        SDL_RenderCopyEx(renderer, pipeTexture, &topPipeStart, &topPipeEnd, 180.0, nullptr, SDL_FLIP_NONE);
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
        int drawHeight = std::min(pipeTextureHeight, remainingHeight);
        SDL_Rect bottomPipeSrc = {0, 0,pipeWidth, drawHeight};
        SDL_Rect bottomPipeDst = {pipe.x, y + totalHeightDrawn, pipeWidth, drawHeight};
        SDL_RenderCopy(renderer, pipeTexture, &bottomPipeSrc, &bottomPipeDst);
        totalHeightDrawn += drawHeight;
    }
}
int PipeManager::getPipeWidth() const {
    return pipeWidth;
}