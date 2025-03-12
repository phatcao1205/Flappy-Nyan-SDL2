#include "Game.h"
#include <iostream>
#include <random>

void Game::spawnPipe() {
    // Tạo ống ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, WINDOW_HEIGHT - PIPE_GAP - 100);
    
    Pipe pipe;
    pipe.x = WINDOW_WIDTH;          // Đặt ống ở bên phải cửa sổ
    pipe.topHeight = dis(gen);      // Chiều cao phần trên ngẫu nhiên
    pipe.passed = false;            // Chưa vượt qua
    pipes.push_back(pipe);          // Thêm ống vào danh sách
}

bool Game::checkCollision() {
    // Kiểm tra va chạm với biên cửa sổ
    if (bird.y < 0 || bird.y + BIRD_SIZE > WINDOW_HEIGHT) {
        return true;
    }

    // Kiểm tra va chạm với ống
    for (const auto& pipe : pipes) {
        if (bird.x + BIRD_SIZE > pipe.x && bird.x < pipe.x + PIPE_WIDTH) {
            if (bird.y < pipe.topHeight || bird.y + BIRD_SIZE > pipe.topHeight + PIPE_GAP) {
                return true;
            }
        }
    }
    return false;
}

Game::Game() {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Khởi tạo SDL thất bại: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bird.x = WINDOW_WIDTH / 4;      // Đặt chim ở giữa bên trái
    bird.y = WINDOW_HEIGHT / 2;     // Đặt chim ở giữa chiều cao
    bird.velocity = 0;              // Vận tốc ban đầu
    
    running = true;                 // Trò chơi bắt đầu
    score = 0;                      // Điểm ban đầu
    spawnPipe();                    // Tạo ống đầu tiên
}

Game::~Game() {
    // Giải phóng tài nguyên
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;        // Thoát khi đóng cửa sổ
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            bird.velocity = -10;    // Nhảy khi nhấn SPACE
        }
    }
}

void Game::update(float deltaTime) {
    // Cập nhật trạng thái chim
    bird.velocity += 0.5f;          // Áp dụng trọng lực
    bird.y += bird.velocity;

    // Cập nhật trạng thái ống
    for (auto& pipe : pipes) {
        pipe.x -= 2;                // Di chuyển ống sang trái
        
        // Cộng điểm khi vượt qua ống
        if (!pipe.passed && bird.x > pipe.x + PIPE_WIDTH) {
            pipe.passed = true;
            score++;
        }
    }

    // Xóa ống đã ra khỏi màn hình và tạo ống mới
    if (!pipes.empty() && pipes.front().x < -PIPE_WIDTH) {
        pipes.erase(pipes.begin());
    }
    if (pipes.back().x < WINDOW_WIDTH - 300) {
        spawnPipe();
    }

    // Kiểm tra va chạm
    if (checkCollision()) {
        running = false;            // Kết thúc trò chơi khi va chạm
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 191, 255, 255); // Màu nền trời xanh
    SDL_RenderClear(renderer);

    // Vẽ chim
    SDL_Rect birdRect = {static_cast<int>(bird.x), static_cast<int>(bird.y), BIRD_SIZE, BIRD_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Màu chim vàng
    SDL_RenderFillRect(renderer, &birdRect);

    // Vẽ ống
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Màu ống xanh lá
    for (const auto& pipe : pipes) {
        SDL_Rect topPipe = {static_cast<int>(pipe.x), 0, PIPE_WIDTH, pipe.topHeight};
        SDL_Rect bottomPipe = {static_cast<int>(pipe.x), pipe.topHeight + PIPE_GAP, 
                             PIPE_WIDTH, WINDOW_HEIGHT - (pipe.topHeight + PIPE_GAP)};
        SDL_RenderFillRect(renderer, &topPipe);
        SDL_RenderFillRect(renderer, &bottomPipe);
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        handleEvents();             // Xử lý sự kiện
        update(deltaTime);          // Cập nhật trạng thái
        render();                   // Vẽ màn hình

        SDL_Delay(16);              // Giới hạn ~60 FPS
    }
    std::cout << "Trò chơi kết thúc! Điểm: " << score << std::endl;
}