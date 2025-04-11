#ifndef PIPE_H
#define PIPE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Pipe {
    int x, height;
};

class PipeManager {
public:
    PipeManager(SDL_Renderer* renderer);
    ~PipeManager();
    void render(SDL_Renderer* renderer, const Pipe& pipe, int baseHeight);
    int getPipeWidth() const;
private:
    SDL_Texture* pipeTexture;
    int pipeWidth;
};

#endif