// game.h
#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "pipe.h"
#include "sound.h"
#include "background.h"
#include "menu.h"
#include "gamestate.h"
#include "constants.h"
#include "bird.h" 

// Forward declaration
struct Bird;
class BirdManager;

// Lớp Game quản lý toàn bộ logic của trò chơi
class Game {
public:
    // Hàm khởi tạo Game
    Game();
    // Hàm hủy Game
    ~Game();
    // Chạy vòng lặp chính của trò chơi
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Bird* bird;
    std::vector<Pipe> pipes;
    bool running;
    GameState gameState;
    Sound sound;
    int score;
    std::vector<bool> pipePassed;
    SDL_Texture* digitTextures[10];
    Background* background;
    PipeManager* pipeManager;
    BirdManager* birdManager;
    Menu* menu;
    float delayTimer;
    SDL_Texture* gameOverTexture;
    SDL_Rect gameOverRect;

    SDL_Texture* musicEnabledTexture;
    SDL_Texture* musicDisabledTexture;
    SDL_Rect musicIconRect;

    SDL_Texture* soundEnabledTexture;
    SDL_Texture* soundDisabledTexture;
    SDL_Rect soundIconRect;

    SDL_Texture* settingTexture;
    SDL_Texture* backTexture;
    SDL_Rect settingRect;

    SDL_Texture* settingCostumesTexture; 
    SDL_Texture* settingMiscTexture;     
    SDL_Rect settingCostumesRect;        
    SDL_Rect settingMiscRect;        
    
    

    bool isBackgroundMusicEnabled;
    bool isSoundEnabled;
    bool isSettingEnabled;
    bool costume;

    // Tải một texture từ đường dẫn tệp
    SDL_Texture* loadTexture(const std::string& filePath);
    // Xử lý các sự kiện đầu vào (bàn phím, chuột, thoát)
    void handleEvents();
    // Cập nhật trạng thái trò chơi (vị trí, va chạm, điểm số)
    void update();
    // Vẽ các thành phần trò chơi lên màn hình
    void render();
    // Khởi tạo vị trí ban đầu của các ống
    void initPipes();
    // Tải các texture chữ số 0-9
    void loadDigitTextures();
    // Vẽ điểm số hiện tại lên màn hình
    void renderScore();
    // Khởi động lại trò chơi về trạng thái ban đầu/menu
    void restart();
};

#endif // GAME_H