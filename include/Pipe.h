#ifndef PIPE_H
#define PIPE_H

// Cấu trúc đại diện cho ống
struct Pipe {
    float x;         // Tọa độ x của ống
    int topHeight;   // Chiều cao phần trên của ống
    bool passed;     // Đã vượt qua ống chưa
};

#endif