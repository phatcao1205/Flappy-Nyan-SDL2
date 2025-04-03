// pipe.h
#ifndef PIPE_H
#define PIPE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Cấu trúc Pipe đại diện cho vị trí và chiều cao của một ống trên
struct Pipe {
    int x, height;
};

// Lớp PipeManager quản lý texture, logic va chạm và vẽ ống
class PipeManager {
public:
    // Hàm khởi tạo PipeManager
    PipeManager(SDL_Renderer* renderer);
    // Hàm hủy PipeManager
    ~PipeManager();
    // Kiểm tra va chạm giữa hình chữ nhật của chim và một cặp ống
    bool checkCollision(int bx, int by, int bw, int bh, int px, int ph);
    // Vẽ một cặp ống (trên và dưới) lên renderer
    void render(SDL_Renderer* renderer, const Pipe& pipe, int baseHeight);

private:
    SDL_Texture* pipeTexture;
    int pipeWidth;
};

#endif // PIPE_H