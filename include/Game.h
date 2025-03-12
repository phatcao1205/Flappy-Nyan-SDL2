#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "Bird.h"
#include "Pipe.h"
#include "Constants.h"

// Lớp chính quản lý trò chơi
class Game {
private:
    SDL_Window* window;    // Cửa sổ SDL
    SDL_Renderer* renderer; // Bộ render SDL
    Bird bird;             // Đối tượng chim
    std::vector<Pipe> pipes; // Danh sách các ống
    bool running;          // Trạng thái trò chơi (đang chạy hay không)
    int score;             // Điểm số

    // Hàm tạo ống ngẫu nhiên
    void spawnPipe();
    // Hàm kiểm tra va chạm
    bool checkCollision();

public:
    Game();                // Hàm tạo
    ~Game();               // Hàm hủy
    void handleEvents();   // Xử lý sự kiện
    void update(float deltaTime); // Cập nhật trạng thái
    void render();         // Vẽ đồ họa
    void run();            // Chạy trò chơi
};

#endif