// game.cpp
#include "game.h"
#include "constants.h"
#include "bird.h"
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_ttf.h>
#include <string>

// Hàm khởi tạo trò chơi
Game::Game() {
    SDL_Init(SDL_INIT_VIDEO);  // Khởi tạo SDL
    // Tạo cửa sổ trò chơi
    window = SDL_CreateWindow("Flappy Nyan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // Tạo renderer để vẽ
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // Tạo icon cho game
    SDL_Surface* icon = IMG_Load("assets/icon.png");
    SDL_SetWindowIcon(window,icon);
    // Khởi tạo đối tượng mèo (cấp phát động)
    bird = new Bird();
    bird->rect = {100, SCREEN_HEIGHT / 2, 80, 40};  // Kích thước mèo (có thể điều chỉnh nếu cần)
    bird->velocity = 0;
    bird->angle = 0.0;  // Khởi tạo góc xoay ban đầu
    running = true;
    gameState = MENU;  // Bắt đầu ở trạng thái menu
    score = 0;  // Khởi tạo điểm số
    delayTimer = 0.0f;  // Khởi tạo bộ đếm thời gian delay

    srand(time(0));  // Khởi tạo seed cho số ngẫu nhiên

    // Tải các hình ảnh chữ số
    loadDigitTextures();

    // Khởi tạo background trước khi gọi initPipes()
    background = new Background(renderer);

    // Khởi tạo các ống
    initPipes();

    // Khởi tạo pipe manager và bird manager
    pipeManager = new PipeManager(renderer);
    birdManager = new BirdManager(renderer);

    // Khởi tạo menu
    menu = new Menu(renderer);

    // Tải gameover.png
    SDL_Surface* gameOverSurface = IMG_Load("assets/gameover.png");
        gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
        SDL_FreeSurface(gameOverSurface);

    // Lấy kích thước gốc của gameover.png và căn giữa
    int width = 0, height = 0;
    SDL_QueryTexture(gameOverTexture, nullptr, nullptr, &width, &height);
    gameOverRect.w = width;
    gameOverRect.h = height;
    gameOverRect.x = (SCREEN_WIDTH - gameOverRect.w) / 2;
    gameOverRect.y = (SCREEN_HEIGHT - gameOverRect.h) / 3;
    

    isSoundEnabled=true;
    // Khởi tạo lại trạng thái âm thanh
    SDL_Surface* soundEnabledSurface=IMG_Load("assets/sound-enabled.png");
    soundEnabledTexture = SDL_CreateTextureFromSurface(renderer, soundEnabledSurface);

    SDL_Surface* soundDisabledSurface=IMG_Load("assets/sound-disabled.png");
    soundDisabledTexture = SDL_CreateTextureFromSurface(renderer, soundDisabledSurface);

    soundIconRect.w=50;
    soundIconRect.h=50;
    soundIconRect.x = SCREEN_WIDTH - soundIconRect.w - 10;
    soundIconRect.y = SCREEN_HEIGHT - soundIconRect.h - 10;

    // Khởi tạo lại trạng thái âm nhạc
    
    isBackgroundMusicEnabled = true;
    SDL_Surface* musicEnabledSurface=IMG_Load("assets/music-enabled.png");
    musicEnabledTexture = SDL_CreateTextureFromSurface(renderer, musicEnabledSurface);

    SDL_Surface* musicDisabledSurface=IMG_Load("assets/music-disabled.png");
    musicDisabledTexture = SDL_CreateTextureFromSurface(renderer, musicDisabledSurface);

    musicIconRect.w=50;
    musicIconRect.h=50;
    musicIconRect.x = SCREEN_WIDTH - musicIconRect.w - soundIconRect.w - 20;
    musicIconRect.y = SCREEN_HEIGHT - musicIconRect.h - 10;

    sound.playBackgroundMusic();



    SDL_Surface* settingSurface=IMG_Load("assets/setting.png");
    SDL_Surface* backSurface=IMG_Load("assets/back.png");
    settingTexture = SDL_CreateTextureFromSurface(renderer, settingSurface);
    backTexture = SDL_CreateTextureFromSurface(renderer, backSurface); 
    settingRect.w = 50;
    settingRect.h = 50;
    settingRect.x = 10;
    settingRect.y = SCREEN_HEIGHT - settingRect.h - 10;
    isSettingEnabled = false;
}

// Hàm hủy, dọn dẹp tài nguyên
Game::~Game() {
    // Giải phóng các texture chữ số
    for (int i = 0; i < 10; i++) {
            SDL_DestroyTexture(digitTextures[i]);
    }
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(soundEnabledTexture);
    SDL_DestroyTexture(soundDisabledTexture);
    delete bird;        // Giải phóng đối tượng mèo
    delete background;  // Giải phóng background
    delete pipeManager; // Giải phóng pipe manager
    delete birdManager; // Giải phóng bird manager
    delete menu;        // Giải phóng menu
    SDL_DestroyRenderer(renderer);  // Hủy renderer
    SDL_DestroyWindow(window);      // Hủy cửa sổ
    SDL_Quit();                     // Thoát SDL
}

// Tải các hình ảnh chữ số từ 0-9
void Game::loadDigitTextures() {
    for (int i = 0; i < 10; i++) {
        std::string path = "assets/" + std::to_string(i) + ".png";
        SDL_Surface* surface = IMG_Load(path.c_str());
        digitTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

// Khởi tạo các ống ban đầu
void Game::initPipes() {
    pipes.clear();
    pipePassed.clear();
    int baseHeight = background->getBaseHeight();
    int maxPipeHeight = SCREEN_HEIGHT - baseHeight - PIPE_GAP - MIN_BOTTOM_PIPE_HEIGHT - 50;  // Đảm bảo ống dưới có chiều cao tối thiểu
    for (int i = 0; i < 3; i++) {
        // Tạo chiều cao ngẫu nhiên cho ống, đảm bảo ống dưới có chiều cao tối thiểu
        int height = rand() % (maxPipeHeight - 50 + 1) + 50;
        // Thêm ống vào danh sách, cách nhau 300 pixel
        pipes.push_back({SCREEN_WIDTH + i * 300, height});
        pipePassed.push_back(false);  // Khởi tạo trạng thái vượt qua
    }
}

// Khởi động lại trò chơi
void Game::restart() {
    // Đặt lại vị trí, vận tốc và góc xoay của mèo
    bird->rect = {100, SCREEN_HEIGHT / 2, 80, 40};
    bird->velocity = 0;
    bird->angle = 0.0;  // Đặt lại góc xoay

    // Đặt lại trạng thái game
    gameState = MENU;  // Quay lại trạng thái menu
    score = 0;
    delayTimer = 0.0f;  // Đặt lại bộ đếm thời gian delay

    // Khởi tạo lại các ống
    initPipes();
    
    // Khởi tạo lại trạng thái âm thanh
    SDL_Surface* soundEnabledSurface=IMG_Load("assets/sound-enabled.png");
    soundEnabledTexture = SDL_CreateTextureFromSurface(renderer, soundEnabledSurface);
    soundIconRect.w=50;
    soundIconRect.h=50;
    soundIconRect.x = SCREEN_WIDTH - soundIconRect.w - 10;
    soundIconRect.y = SCREEN_HEIGHT - soundIconRect.h - 10;
    SDL_Surface* soundDisabledSurface=IMG_Load("assets/sound-disabled.png");
    soundDisabledTexture = SDL_CreateTextureFromSurface(renderer, soundDisabledSurface);

    SDL_Surface* settingSurface=IMG_Load("assets/setting.png");
    SDL_Surface* backSurface=IMG_Load("assets/back.png");
    settingTexture = SDL_CreateTextureFromSurface(renderer, settingSurface);
    backTexture = SDL_CreateTextureFromSurface(renderer, backSurface); 
    settingRect.w = 50;
    settingRect.h = 50;
    settingRect.x = 10;
    settingRect.y = SCREEN_HEIGHT - settingRect.h - 10;


}

// Xử lý các sự kiện như nhấn phím hoặc thoát game
void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;  // Thoát game nếu nhấn nút thoát
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= musicIconRect.x && mouseX <= musicIconRect.x + musicIconRect.w &&
                mouseY >= musicIconRect.y && mouseY <= musicIconRect.y + musicIconRect.h) {
                isBackgroundMusicEnabled = !isBackgroundMusicEnabled;
                if (isBackgroundMusicEnabled) {
                    sound.playBackgroundMusic();
                } else {
                    sound.stopBackgroundMusic();
                }
            }
            if (mouseX >= soundIconRect.x && mouseX <= soundIconRect.x + soundIconRect.w &&
                mouseY >= soundIconRect.y && mouseY <= soundIconRect.y + soundIconRect.h) {
                isSoundEnabled = !isSoundEnabled;
                if (isSoundEnabled) {
                    sound.playPointSound(isSoundEnabled);
                }
            }
            if (mouseX >= settingRect.x && mouseX <= settingRect.x + settingRect.w &&
                mouseY >= settingRect.y && mouseY <= settingRect.y + settingRect.h) {
                    isSettingEnabled = !isSettingEnabled;
                if (isSettingEnabled) {
                    gameState=SETTING;
                }
                else gameState=MENU;
            }
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                if (gameState == MENU) {
                    gameState = STARTING;  // Chuyển sang trạng thái delay (STARTING)
                    sound.playWingSound(isSoundEnabled); // Phát âm thanh khi nhảy
                } else if (gameState == PLAYING) {
                    bird->velocity = JUMP_STRENGTH;  // Nhảy khi nhấn phím Space
                    sound.playWingSound(isSoundEnabled);           // Phát âm thanh khi nhảy
                }
            }
            if (event.key.keysym.sym == SDLK_r && gameState == GAME_OVER) {
                restart();  // Khởi động lại game khi nhấn phím R ở trạng thái game over
            }
        }
    }
}

// Cập nhật trạng thái trò chơi
void Game::update() {
    // Cập nhật vị trí của background và base trong cả MENU, STARTING và PLAYING
    if (gameState != GAME_OVER) {
        background->update();
    }

    if (gameState == MENU||gameState == SETTING) {
        // Ở trạng thái menu, chỉ cập nhật animation và hiệu ứng lơ lửng của mèo
        birdManager->updateBird(*bird, gameState);
        return;
    }

    if (gameState == STARTING) {
        // Ở trạng thái STARTING, tiếp tục hiệu ứng lơ lửng cho mèo
        birdManager->updateBird(*bird, MENU);  // Sử dụng logic của MENU để giữ mèo lơ lửng

        // Cập nhật vị trí các ống
        for (size_t i = 0; i < pipes.size(); i++) {
            auto& pipe = pipes[i];
            pipe.x -= PIPE_SPEED;  // Di chuyển ống sang trái
            if (pipe.x + PIPE_WIDTH < 0) {  // Nếu ống ra khỏi màn hình
                pipe.x = SCREEN_WIDTH;  // Đặt lại vị trí ở bên phải
                int baseHeight = background->getBaseHeight();
                int maxPipeHeight = SCREEN_HEIGHT - baseHeight - PIPE_GAP - MIN_BOTTOM_PIPE_HEIGHT - 50;
                pipe.height = rand() % (maxPipeHeight - 50 + 1) + 50;  // Tạo chiều cao ngẫu nhiên
                pipePassed[i] = false;  // Đặt lại trạng thái vượt qua
            }
        }

        // Tăng bộ đếm thời gian delay
        delayTimer += 0.016f;  // 16ms mỗi frame (60 FPS)
        if (delayTimer >= 0.5f) {  // Sau 0,5 giây
            gameState = PLAYING;   // Chuyển sang trạng thái PLAYING
        }
        return;
    }

    if (gameState == GAME_OVER) {
        return;  // Không cập nhật nếu trò chơi đã kết thúc
    }


    birdManager->updateBird(*bird, gameState);  // Cập nhật vị trí, animation và góc xoay của mèo

    // Cập nhật vị trí các ống
    for (size_t i = 0; i < pipes.size(); i++) {
        auto& pipe = pipes[i];
        pipe.x -= PIPE_SPEED;  // Di chuyển ống sang trái
        if (pipe.x + PIPE_WIDTH < 0) {  // Nếu ống ra khỏi màn hình
            pipe.x = SCREEN_WIDTH;  // Đặt lại vị trí ở bên phải
            int baseHeight = background->getBaseHeight();
            int maxPipeHeight = SCREEN_HEIGHT - baseHeight - PIPE_GAP - MIN_BOTTOM_PIPE_HEIGHT - 50;
            pipe.height = rand() % (maxPipeHeight - 50 + 1) + 50;  // Tạo chiều cao ngẫu nhiên
            pipePassed[i] = false;  // Đặt lại trạng thái vượt qua
        }

        // Kiểm tra nếu mèo vượt qua ống
        if (!pipePassed[i] && bird->rect.x > pipe.x + PIPE_WIDTH) {
            pipePassed[i] = true;  // Đánh dấu đã vượt qua
            score++;               // Tăng điểm
            sound.playPointSound(isSoundEnabled);  // Phát âm thanh ghi điểm
        }

        // Kiểm tra va chạm giữa mèo và ống
        if (pipeManager->checkCollision(bird->rect.x, bird->rect.y, bird->rect.w, bird->rect.h, pipe.x, pipe.height)) {
            sound.playHitSound(isSoundEnabled);  // Phát âm thanh va chạm
            sound.playDieSound(isSoundEnabled);  // Phát âm thanh thua
            gameState = GAME_OVER;  // Chuyển sang trạng thái game over
        }
    }

    // Kết thúc game nếu mèo chạm đỉnh hoặc chạm base
    int baseHeight = background->getBaseHeight();
    if (bird->rect.y + bird->rect.h > SCREEN_HEIGHT - baseHeight) {  // Mèo chạm base
        bird->rect.y = SCREEN_HEIGHT - baseHeight - bird->rect.h;  // Đặt mèo nằm trên base
        bird->velocity = 0;  // Dừng vận tốc
        sound.playHitSound(isSoundEnabled);  // Phát âm thanh va chạm
        sound.playDieSound(isSoundEnabled);  // Phát âm thanh thua
        gameState = GAME_OVER;  // Chuyển sang trạng thái game over
    }
    if (bird->rect.y < 0) {  // Mèo chạm đỉnh màn hình
        sound.playHitSound(isSoundEnabled);  // Phát âm thanh va chạm
        sound.playDieSound(isSoundEnabled);  // Phát âm thanh thua
        gameState = GAME_OVER;  // Chuyển sang trạng thái game over
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
            SDL_Rect digitRect = {static_cast<int>(startX + i * digitWidth), startY, digitWidth, digitHeight};
            SDL_RenderCopy(renderer, digitTextures[digit], nullptr, &digitRect);
        }
    }
}

// Vẽ các thành phần lên màn hình
void Game::render() {
    // Vẽ nền và mặt đất
    background->render();

    // Vẽ mèo bằng birdManager
    birdManager->render(renderer, *bird);

    if (gameState == MENU) {
        // Ở trạng thái menu, chỉ vẽ menu
        menu->render();
        if (isBackgroundMusicEnabled && musicEnabledTexture) {
            SDL_RenderCopy(renderer, musicEnabledTexture, nullptr, &musicIconRect);
        } else if (!isBackgroundMusicEnabled && musicDisabledTexture) {
            SDL_RenderCopy(renderer, musicDisabledTexture, nullptr, &musicIconRect);
        }
        if (isSoundEnabled && soundEnabledTexture) {
            SDL_RenderCopy(renderer, soundEnabledTexture, nullptr, &soundIconRect);
        } else if (!isSoundEnabled && soundDisabledTexture) {
            SDL_RenderCopy(renderer, soundDisabledTexture, nullptr, &soundIconRect);
        }
        SDL_RenderCopy(renderer, settingTexture, nullptr, &settingRect);
    } else if (gameState == STARTING || gameState == PLAYING) {
        // Ở trạng thái STARTING hoặc PLAYING, vẽ các ống và điểm số
        int baseHeight = background->getBaseHeight();
        for (const auto& pipe : pipes) {
            pipeManager->render(renderer, pipe, baseHeight);
        }
        renderScore();
    } else if (gameState == GAME_OVER) {
        // Ở trạng thái game over, vẽ các ống, điểm số và gameover.png
        int baseHeight = background->getBaseHeight();
        for (const auto& pipe : pipes) {
            pipeManager->render(renderer, pipe, baseHeight);
        }
        renderScore();
        // Hiển thị gameover.png
        if (gameOverTexture) {
            SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
        }
    }
    else if(gameState == SETTING){
        if (isBackgroundMusicEnabled && musicEnabledTexture) {
            SDL_RenderCopy(renderer, musicEnabledTexture, nullptr, &musicIconRect);
        } else if (!isBackgroundMusicEnabled && musicDisabledTexture) {
            SDL_RenderCopy(renderer, musicDisabledTexture, nullptr, &musicIconRect);
        }
        if (isSoundEnabled && soundEnabledTexture) {
            SDL_RenderCopy(renderer, soundEnabledTexture, nullptr, &soundIconRect);
        } else if (!isSoundEnabled && soundDisabledTexture) {
            SDL_RenderCopy(renderer, soundDisabledTexture, nullptr, &soundIconRect);
        }
        if(backTexture) SDL_RenderCopy(renderer, backTexture, nullptr, &settingRect);
    }
    SDL_RenderPresent(renderer);  // Hiển thị khung hình
}

// Hàm chạy vòng lặp chính của trò chơi
void Game::run() {
    while (running) {
        handleEvents();  // Xử lý sự kiện
        update();        // Cập nhật trạng thái
        render();        // Vẽ khung hình
        SDL_Delay(16);   // Tạm dừng 16ms để đạt khoảng 60 FPS
    }
}