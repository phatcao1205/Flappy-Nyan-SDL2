// game.cpp
#include "game.h"
#include "constants.h"
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_ttf.h>
#include <string>

// Hàm khởi tạo trò chơi
Game::Game() {
    SDL_Init(SDL_INIT_VIDEO);  // Khởi tạo SDL
    // Tạo cửa sổ trò chơi
    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // Tạo renderer để vẽ
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Khởi tạo vị trí ban đầu của chim
    bird.rect = {100, SCREEN_HEIGHT / 2, 40, 40};
    bird.velocity = 0;
    running = true;
    gameOver = false;  // Khởi tạo trạng thái game over
    score = 0;  // Khởi tạo điểm số

    srand(time(0));  // Khởi tạo seed cho số ngẫu nhiên
    initPipes();     // Khởi tạo các ống
    pipePassed.resize(pipes.size(), false);  // Khởi tạo mảng theo dõi trạng thái vượt qua ống

    // Tải các hình ảnh chữ số
    loadDigitTextures();

    // Khởi tạo background
    background = new Background(renderer);
}

// Hàm hủy, dọn dẹp tài nguyên
Game::~Game() {
    // Giải phóng các texture chữ số
    for (int i = 0; i < 10; i++) {
        if (digitTextures[i]) {
            SDL_DestroyTexture(digitTextures[i]);
        }
    }
    delete background;  // Giải phóng background
    SDL_DestroyRenderer(renderer);  // Hủy renderer
    SDL_DestroyWindow(window);      // Hủy cửa sổ
    SDL_Quit();                     // Thoát SDL
}

// Tải các hình ảnh chữ số từ 0-9
void Game::loadDigitTextures() {
    for (int i = 0; i < 10; i++) {
        std::string path = "assets/" + std::to_string(i) + ".png";
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            SDL_Log("Không thể tải hình ảnh %s: %s", path.c_str(), IMG_GetError());
            digitTextures[i] = nullptr;
            continue;
        }
        digitTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!digitTextures[i]) {
            SDL_Log("Không thể tạo texture từ %s: %s", path.c_str(), SDL_GetError());
        }
    }
}

// Khởi tạo các ống ban đầu
void Game::initPipes() {
    for (int i = 0; i < 3; i++) {
        // Tạo chiều cao ngẫu nhiên cho ống
        int height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
        // Thêm ống vào danh sách, cách nhau 300 pixel
        pipes.push_back({SCREEN_WIDTH + i * 300, height});
    }
}

// Xử lý các sự kiện như nhấn phím hoặc thoát game
void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;  // Thoát game nếu nhấn nút thoát
        if (!gameOver && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            bird.velocity = JUMP_STRENGTH;  // Nhảy khi nhấn phím Space
            sound.playWingSound();          // Phát âm thanh khi nhảy
        }
    }
}

// Cập nhật trạng thái trò chơi
void Game::update() {
    if (gameOver) return;  // Không cập nhật nếu trò chơi đã kết thúc

    updateBird(bird);  // Cập nhật vị trí chim

    // Cập nhật vị trí các ống
    for (size_t i = 0; i < pipes.size(); i++) {
        auto& pipe = pipes[i];
        pipe.x -= PIPE_SPEED;  // Di chuyển ống sang trái
        if (pipe.x + PIPE_WIDTH < 0) {  // Nếu ống ra khỏi màn hình
            pipe.x = SCREEN_WIDTH;  // Đặt lại vị trí ở bên phải
            pipe.height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;  // Tạo chiều cao ngẫu nhiên
            pipePassed[i] = false;  // Đặt lại trạng thái vượt qua
        }

        // Kiểm tra nếu chim vượt qua ống
        if (!pipePassed[i] && bird.rect.x > pipe.x + PIPE_WIDTH) {
            pipePassed[i] = true;  // Đánh dấu đã vượt qua
            score++;               // Tăng điểm
            sound.playPointSound();  // Phát âm thanh ghi điểm
        }

        // Kiểm tra va chạm giữa chim và ống
        if (checkCollision(bird.rect.x, bird.rect.y, bird.rect.w, bird.rect.h, pipe.x, pipe.height)) {
            sound.playHitSound();  // Phát âm thanh va chạm
            sound.playDieSound();  // Phát âm thanh thua
            gameOver = true;       // Đặt trạng thái game over
        }
    }

    // Kết thúc game nếu chim chạm đỉnh hoặc chạm base
    int baseHeight = background->getBaseHeight();
    if (bird.rect.y + bird.rect.h > SCREEN_HEIGHT - baseHeight) {  // Chim chạm base
        bird.rect.y = SCREEN_HEIGHT - baseHeight - bird.rect.h;  // Đặt chim nằm trên base
        bird.velocity = 0;  // Dừng vận tốc
        sound.playHitSound();  // Phát âm thanh va chạm
        sound.playDieSound();  // Phát âm thanh thua
        gameOver = true;       // Đặt trạng thái game over
    }
    if (bird.rect.y < 0) {  // Chim chạm đỉnh màn hình
        sound.playHitSound();  // Phát âm thanh va chạm
        sound.playDieSound();  // Phát âm thanh thua
        gameOver = true;       // Đặt trạng thái game over
    }
}

// Vẽ điểm số lên màn hình
void Game::renderScore() {
    // Chuyển điểm số thành chuỗi để lấy từng chữ số
    std::string scoreStr = std::to_string(score);
    int digitWidth = 30;  // Chiều rộng của mỗi chữ số
    int digitHeight = 50; // Chiều cao của mỗi chữ số
    int totalWidth = scoreStr.length() * digitWidth;  // Tổng chiều rộng của điểm số
    int startX = (SCREEN_WIDTH - totalWidth) / 2;    // Tọa độ x để căn giữa
    int startY = 50;                                 // Tọa độ y (gần đỉnh màn hình)

    // Vẽ từng chữ số
    for (size_t i = 0; i < scoreStr.length(); i++) {
        int digit = scoreStr[i] - '0';  // Chuyển ký tự thành số (0-9)
        if (digitTextures[digit]) {
            SDL_Rect digitRect = {static_cast<int>(startX + i * digitWidth), startY, digitWidth, digitHeight}; // Ép kiểu để tránh cảnh báo
            SDL_RenderCopy(renderer, digitTextures[digit], nullptr, &digitRect);
        }
    }
}

// Vẽ các thành phần lên màn hình
void Game::render() {
    // Vẽ nền và mặt đất
    background->render();

    // Vẽ chim
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Đặt màu cho chim (vàng)
    SDL_RenderFillRect(renderer, &bird.rect);  // Vẽ chim

    // Vẽ các ống
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Đặt màu cho ống (xanh lá)
    for (const auto& pipe : pipes) {
        // Vẽ ống trên
        SDL_Rect topPipe = {pipe.x, 0, PIPE_WIDTH, pipe.height};
        // Vẽ ống dưới
        SDL_Rect bottomPipe = {pipe.x, pipe.height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - pipe.height - PIPE_GAP};
        SDL_RenderFillRect(renderer, &topPipe);  // Vẽ ống trên
        SDL_RenderFillRect(renderer, &bottomPipe);  // Vẽ ống dưới
    }

    // Vẽ điểm số
    renderScore();

    // Nếu trò chơi kết thúc, hiển thị thông báo "Game Over"
    if (gameOver) {
        // Khởi tạo SDL_ttf để hiển thị văn bản
        TTF_Init();
        TTF_Font* font = TTF_OpenFont("arial.ttf", 48);  // Cần có file font
        if (font) {
            SDL_Color textColor = {255, 0, 0, 255};  // Màu đỏ
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 24, 200, 48};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            // Giải phóng tài nguyên
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
            TTF_CloseFont(font);
        }
        TTF_Quit();
    }

    SDL_RenderPresent(renderer);  // Hiển thị khung hình
}

// Hàm chạy vòng lặp chính của trò chơi
void Game::run() {
    while (running) {
        handleEvents();  // Xử lý sự kiện
        update();        // Cập nhật trạng thái (chỉ khi không game over)
        render();        // Vẽ khung hình
        SDL_Delay(16);   // Tạm dừng 16ms để đạt khoảng 60 FPS
    }
}