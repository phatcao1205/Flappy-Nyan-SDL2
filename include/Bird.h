// bird.h
#ifndef BIRD_H
#define BIRD_H

#include <SDL2/SDL.h>

// Cấu trúc Bird đại diện cho con chim trong trò chơi
struct Bird {
    SDL_Rect rect;  // Hình chữ nhật đại diện cho chim (vị trí và kích thước)
    int velocity;   // Vận tốc theo trục y của chim
};

// Hàm cập nhật vị trí và vận tốc của chim
void updateBird(Bird& bird);

#endif