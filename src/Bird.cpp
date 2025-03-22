// bird.cpp
#include "bird.h"
#include "constants.h"

// Hàm cập nhật vị trí và vận tốc của chim
void updateBird(Bird& bird) {
    bird.velocity += GRAVITY;  // Tăng vận tốc theo trọng lực
    bird.rect.y += bird.velocity;  // Cập nhật vị trí y của chim dựa trên vận tốc
}