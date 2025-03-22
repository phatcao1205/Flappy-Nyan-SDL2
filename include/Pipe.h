// pipe.h
#ifndef PIPE_H
#define PIPE_H

#include <SDL2/SDL.h>

// Cấu trúc Pipe đại diện cho một cặp ống (trên và dưới)
struct Pipe {
    int x, height;  // Tọa độ x và chiều cao của ống trên
};

// Hàm kiểm tra va chạm giữa chim và ống
bool checkCollision(int bx, int by, int bw, int bh, int px, int ph);

#endif