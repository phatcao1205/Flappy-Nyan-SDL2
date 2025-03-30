// game.h
#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "pipe.h"
#include "sound.h"
#include "background.h"
#include "menu.h"
#include "gamestate.h"

// Forward declaration cho Bird và BirdManager
struct Bird;
class BirdManager;

// Lớp Game quản lý toàn bộ logic của trò chơi
class Game {
public:
    Game();   // Hàm khởi tạo
    ~Game();  // Hàm hủy
    void run();  // Hàm chạy trò chơi

private:
    SDL_Window* window;    // Cửa sổ trò chơi
    SDL_Renderer* renderer;  // Renderer để vẽ lên màn hình
    Bird* bird;            // Đối tượng mèo (trước đây là chim)
    std::vector<Pipe> pipes;  // Danh sách các ống
    bool running;          // Trạng thái chạy của cửa sổ
    GameState gameState;   // Trạng thái của trò chơi
    Sound sound;           // Đối tượng quản lý âm thanh
    int score;             // Điểm số của người chơi
    std::vector<bool> pipePassed;  // Theo dõi xem mèo đã vượt qua ống nào chưa
    SDL_Texture* digitTextures[10];  // Mảng lưu texture của các chữ số 0-9
    Background* background;          // Đối tượng quản lý nền và mặt đất
    PipeManager* pipeManager;        // Đối tượng quản lý ống
    BirdManager* birdManager;        // Đối tượng quản lý mèo
    Menu* menu;                      // Đối tượng quản lý menu
    float delayTimer;                // Bộ đếm thời gian delay
    SDL_Texture* gameOverTexture;    // Texture cho gameover.png
    SDL_Rect gameOverRect;           // Vị trí và kích thước của gameover.png
    
    SDL_Texture* musicEnabledTexture;  // Texture cho sound-enabled.png (mới thêm)
    SDL_Texture* musicDisabledTexture; // Texture cho sound-disabled.png (mới thêm)
    SDL_Rect musicIconRect;  

    SDL_Texture* soundEnabledTexture;  // Texture cho sound-enabled.png (mới thêm)
    SDL_Texture* soundDisabledTexture; // Texture cho sound-disabled.png (mới thêm)
    SDL_Rect soundIconRect;            // Vị trí và kích thước của icon âm thanh (mới thêm)
   
    
    SDL_Texture* settingTexture; // Texture cho sound-disabled.png (mới thêm)
    SDL_Texture* backTexture; // Texture cho sound-disabled.png (mới thêm)
    SDL_Rect settingRect;            // Vị trí và kích thước của icon âm thanh (mới thêm)

    bool isBackgroundMusicEnabled;     // Trạng thái nhạc nền (bật/tắt)
    bool isSoundEnabled;
    bool isSettingEnabled;              // Trạng thái của setting

    void handleEvents();  // Xử lý các sự kiện (nhấn phím, thoát game, nhấp chuột, v.v.)
    void update();        // Cập nhật trạng thái trò chơi
    void render();        // Vẽ các thành phần lên màn hình
    void initPipes();     // Khởi tạo các ống ban đầu
    void loadDigitTextures();  // Tải các hình ảnh chữ số
    void renderScore();        // Vẽ điểm số lên màn hình
    void restart();            // Khởi động lại trò chơi
};

#endif