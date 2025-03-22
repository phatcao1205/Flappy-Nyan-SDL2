// pipe.cpp
#include "pipe.h"
#include "constants.h"

// Hàm kiểm tra va chạm giữa chim và ống
// Tham số: bx, by - tọa độ của chim; bw, bh - chiều rộng, chiều cao của chim; px, ph - tọa độ x và chiều cao ống trên
bool checkCollision(int bx, int by, int bw, int bh, int px, int ph) {
    // Kiểm tra nếu chim va chạm với ống theo trục x và y
    return (bx < px + PIPE_WIDTH && bx + bw > px &&
            (by < ph || by + bh > ph + PIPE_GAP));
}