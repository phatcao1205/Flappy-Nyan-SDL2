// game.cpp
#include "game.h"


// Tải một SDL_Texture từ đường dẫn tệp được cung cấp
SDL_Texture* Game::loadTexture(const std::string& filePath) {
    SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

// Hàm khởi tạo Game, thiết lập SDL, cửa sổ, renderer, tải tài sản, khởi tạo đối tượng
Game::Game() {
    window = SDL_CreateWindow("Flappy Nyan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* icon = IMG_Load("assets/icon.png");
    if (icon) { // Kiểm tra icon có được tải thành công không
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    bird = new Bird();
    bird->rect = {100, SCREEN_HEIGHT / 2, 80, 40};
    bird->velocity = 0;
    bird->angle = 0.0;
    running = true;
    gameState = MENU;
    score = 0;
    delayTimer = 0.0f;

    srand(time(0));

    loadDigitTextures();

    background = new Background(renderer);
    initPipes(); // Gọi sau khi background đã có để lấy baseHeight
    pipeManager = new PipeManager(renderer);
    birdManager = new BirdManager(renderer);
    menu = new Menu(renderer);

    gameOverTexture = loadTexture("assets/gameover.png");
    int width = 0, height = 0;
    SDL_QueryTexture(gameOverTexture, nullptr, nullptr, &width, &height);
    gameOverRect = {(SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 3, width, height};

    isSoundEnabled = true;
    soundEnabledTexture = loadTexture("assets/sound-enabled.png");
    soundDisabledTexture = loadTexture("assets/sound-disabled.png");
    soundIconRect = {SCREEN_WIDTH - 50 - 10, SCREEN_HEIGHT - 50 - 10, 50, 50};

    isBackgroundMusicEnabled = true;
    musicEnabledTexture = loadTexture("assets/music-enabled.png");
    musicDisabledTexture = loadTexture("assets/music-disabled.png");
    musicIconRect = {SCREEN_WIDTH - 50 - 50 - 20, SCREEN_HEIGHT - 50 - 10, 50, 50};

    sound.playBackgroundMusic(); // Bắt đầu nhạc nền

    settingTexture = loadTexture("assets/setting.png");
    backTexture = loadTexture("assets/back.png");
    settingRect = {10, SCREEN_HEIGHT - 50 - 10, 50, 50};
    isSettingEnabled = false;

    settingCostumesTexture = loadTexture("assets/setting-costumes.png");
    settingCostumesRect = {60, 20, 250, 60};
    settingMiscTexture = loadTexture("assets/setting-miscellaneous.png");
    settingMiscRect = {380, 20, 350, 60};
    costume=true;
}

// Hàm hủy Game, giải phóng tất cả tài nguyên đã cấp phát
Game::~Game() {
    for (int i = 0; i < 10; i++) {
        SDL_DestroyTexture(digitTextures[i]);
    }
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(soundEnabledTexture);
    SDL_DestroyTexture(soundDisabledTexture);
    SDL_DestroyTexture(musicEnabledTexture);
    SDL_DestroyTexture(musicDisabledTexture);
    SDL_DestroyTexture(settingTexture);
    SDL_DestroyTexture(backTexture);
    SDL_DestroyTexture(settingCostumesTexture);
    SDL_DestroyTexture(settingMiscTexture);

    delete bird;
    delete background;
    delete pipeManager;
    delete birdManager;
    delete menu;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

// Tải các texture cho chữ số từ 0 đến 9
void Game::loadDigitTextures() {
    for (int i = 0; i < 10; i++) {
        std::string path = "assets/" + std::to_string(i) + ".png";
        digitTextures[i] = loadTexture(path);
    }
}

// Khởi tạo hoặc đặt lại vị trí các ống
void Game::initPipes() {
    pipes.clear();
    pipePassed.clear();
    int baseHeight = background->getBaseHeight();
    int maxPipeHeight = SCREEN_HEIGHT - baseHeight - PIPE_GAP - MIN_BOTTOM_PIPE_HEIGHT - 50;
    for (int i = 0; i < 3; i++) {
        int height = rand() % (maxPipeHeight - 50 + 1) + 50;
        pipes.push_back({SCREEN_WIDTH + i * 300, height});
        pipePassed.push_back(false);
    }
}

// Đặt lại trạng thái trò chơi về ban đầu hoặc menu
void Game::restart() {
    bird->rect = {100, SCREEN_HEIGHT / 2, 80, 40};
    bird->velocity = 0;
    bird->angle = 0.0;
    gameState = MENU;
    score = 0;
    delayTimer = 0.0f;
    initPipes();
    isSettingEnabled = false; // Đảm bảo thoát khỏi màn hình setting khi restart
    // Không cần tải lại texture ở đây, chỉ cần đảm bảo trạng thái đúng
}

// Xử lý các sự kiện đầu vào từ người dùng (bàn phím, chuột, đóng cửa sổ)
void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Kiểm tra click vào icon nhạc nền
            if (mouseX >= musicIconRect.x && mouseX <= musicIconRect.x + musicIconRect.w &&
                mouseY >= musicIconRect.y && mouseY <= musicIconRect.y + musicIconRect.h) {
                isBackgroundMusicEnabled = !isBackgroundMusicEnabled;
                if (isBackgroundMusicEnabled) sound.playBackgroundMusic();
                else sound.stopBackgroundMusic();
            }
            // Kiểm tra click vào icon âm thanh
            else if (mouseX >= soundIconRect.x && mouseX <= soundIconRect.x + soundIconRect.w &&
                     mouseY >= soundIconRect.y && mouseY <= soundIconRect.y + soundIconRect.h) {
                isSoundEnabled = !isSoundEnabled;
                // Có thể thêm âm thanh click nhỏ ở đây nếu muốn
            }
            
            else if (mouseX >= settingMiscRect.x && mouseX <= settingMiscRect.x + settingMiscRect.w &&
                     mouseY >= settingMiscRect.y && mouseY <= settingMiscRect.y + settingMiscRect.h) {
                        costume=false;
            }
            else if (mouseX >= settingCostumesRect.x && mouseX <= settingCostumesRect.x + settingCostumesRect.w &&
                     mouseY >= settingCostumesRect.y && mouseY <= settingCostumesRect.y + settingCostumesRect.h) {
                        costume=true;
            }
            // Kiểm tra click vào icon cài đặt/quay lại
            else if (mouseX >= settingRect.x && mouseX <= settingRect.x + settingRect.w &&
                     mouseY >= settingRect.y && mouseY <= settingRect.y + settingRect.h) {
            if(gameState == MENU) gameState = SETTING;
            else if (gameState == SETTING){
                gameState = MENU;
                costume=true;
            }
            isSettingEnabled = (gameState == SETTING); // Cập nhật trạng thái isSettingEnabled
       }
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                if (gameState == MENU) {
                    gameState = STARTING; // Chuyển sang delay trước khi chơi
                    sound.playWingSound(isSoundEnabled);
                } else if (gameState == PLAYING) {
                    bird->velocity = JUMP_STRENGTH;
                    sound.playWingSound(isSoundEnabled);
                }
            }
            if (event.key.keysym.sym == SDLK_r && gameState == GAME_OVER) {
                restart();
            }
        }
    }
}

// Cập nhật logic trò chơi dựa trên trạng thái hiện tại
void Game::update() {
    if (gameState != GAME_OVER) {
        background->update(); // Luôn cập nhật nền trừ khi game over
    }

    if (gameState == MENU || gameState == SETTING) {
        birdManager->updateBird(*bird, gameState); // Chỉ cập nhật hiệu ứng lơ lửng
        return;
    }

    if (gameState == STARTING) {
        birdManager->updateBird(*bird, MENU); // Giữ hiệu ứng lơ lửng
        // Di chuyển ống trong trạng thái chờ
        for (size_t i = 0; i < pipes.size(); i++) {
            pipes[i].x -= PIPE_SPEED;
             if (pipes[i].x + PIPE_WIDTH < 0) {
                pipes[i].x = SCREEN_WIDTH;
                int baseHeight = background->getBaseHeight();
                int maxPipeHeight = SCREEN_HEIGHT - baseHeight - PIPE_GAP - MIN_BOTTOM_PIPE_HEIGHT - 50;
                pipes[i].height = rand() % (maxPipeHeight - 50 + 1) + 50;
                pipePassed[i] = false;
            }
        }
        delayTimer += 0.016f; // Tăng bộ đếm delay
        if (delayTimer >= 0.5f) {
            gameState = PLAYING; // Bắt đầu chơi
        }
        return;
    }

    if (gameState == GAME_OVER) {
        return; // Không làm gì khi game over
    }

    // Trạng thái PLAYING
    birdManager->updateBird(*bird, gameState); // Cập nhật chim với trọng lực

    int baseHeight = background->getBaseHeight();
    // Cập nhật ống và kiểm tra va chạm/điểm
    for (size_t i = 0; i < pipes.size(); i++) {
        pipes[i].x -= PIPE_SPEED;

        // Kiểm tra va chạm
        if (pipeManager->checkCollision(bird->rect.x, bird->rect.y, bird->rect.w, bird->rect.h, pipes[i].x, pipes[i].height)) {
            sound.playHitSound(isSoundEnabled);
            sound.playDieSound(isSoundEnabled);
            gameState = GAME_OVER;
            return; // Thoát sớm khi va chạm
        }

        // Kiểm tra ghi điểm
        if (!pipePassed[i] && bird->rect.x > pipes[i].x + PIPE_WIDTH) {
            pipePassed[i] = true;
            score++;
            sound.playPointSound(isSoundEnabled);
        }

        // Đặt lại ống khi ra khỏi màn hình
        if (pipes[i].x + PIPE_WIDTH < 0) {
            pipes[i].x = SCREEN_WIDTH + (pipes.size() > 1 ? 0 : 150); // Đặt lại vị trí, có thể thêm khoảng cách nhỏ
            int maxPipeHeight = SCREEN_HEIGHT - baseHeight - PIPE_GAP - MIN_BOTTOM_PIPE_HEIGHT - 50;
            pipes[i].height = rand() % (maxPipeHeight - 50 + 1) + 50;
            pipePassed[i] = false;
        }
    }

    // Kiểm tra va chạm với mặt đất và trần nhà
    if (bird->rect.y + bird->rect.h > SCREEN_HEIGHT - baseHeight) {
        bird->rect.y = SCREEN_HEIGHT - baseHeight - bird->rect.h;
        bird->velocity = 0;
        sound.playHitSound(isSoundEnabled);
        sound.playDieSound(isSoundEnabled);
        gameState = GAME_OVER;
    }
    if (bird->rect.y < 0) { // Va chạm trần
        bird->rect.y = 0; // Giữ chim ở mép trên
        bird->velocity = 0; // Ngăn không cho nảy lên nữa
        // Âm thanh va chạm có thể không cần thiết khi chạm trần, tùy thiết kế
        sound.playHitSound(isSoundEnabled);
        sound.playDieSound(isSoundEnabled); // Vẫn tính là thua
        gameState = GAME_OVER;
    }
}

// Vẽ điểm số hiện tại lên màn hình sử dụng các texture chữ số
void Game::renderScore() {
    std::string scoreStr = std::to_string(score);
    int digitWidth = 30;
    int digitHeight = 50;
    int totalWidth = scoreStr.length() * digitWidth;
    int startX = (SCREEN_WIDTH - totalWidth) / 2;
    int startY = 50;

    for (size_t i = 0; i < scoreStr.length(); i++) {
        int digit = scoreStr[i] - '0';
        if (digitTextures[digit]) {
            SDL_Rect digitRect = {startX + static_cast<int>(i * digitWidth), startY, digitWidth, digitHeight};
            SDL_RenderCopy(renderer, digitTextures[digit], nullptr, &digitRect);
        }
    }
}

// Vẽ tất cả các thành phần của trò chơi lên màn hình tùy theo GameState
void Game::render() {
    background->render(); // Vẽ nền và đất trước tiên
    birdManager->render(renderer, *bird); // Vẽ chim

    int baseHeight = background->getBaseHeight();

    if (gameState == MENU) {
        menu->render(); // Vẽ thông điệp menu
        // Vẽ các icon điều khiển
        SDL_RenderCopy(renderer, isBackgroundMusicEnabled ? musicEnabledTexture : musicDisabledTexture, nullptr, &musicIconRect);
        SDL_RenderCopy(renderer, isSoundEnabled ? soundEnabledTexture : soundDisabledTexture, nullptr, &soundIconRect);
        SDL_RenderCopy(renderer, settingTexture, nullptr, &settingRect); // Icon cài đặt
    } else if (gameState == STARTING || gameState == PLAYING) {
        // Vẽ ống khi đang chờ hoặc đang chơi
        for (const auto& pipe : pipes) {
            pipeManager->render(renderer, pipe, baseHeight);
        }
        renderScore(); // Vẽ điểm số
    } else if (gameState == GAME_OVER) {
        // Vẽ ống và điểm số ở trạng thái game over
        for (const auto& pipe : pipes) {
            pipeManager->render(renderer, pipe, baseHeight);
        }
        renderScore();
        // Vẽ thông báo game over
        if (gameOverTexture) {
            SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
        }
         // Có thể thêm nút Restart ở đây
    } else if(gameState == SETTING){
         // Vẽ các icon điều khiển trong màn hình setting
        SDL_RenderCopy(renderer, isBackgroundMusicEnabled ? musicEnabledTexture : musicDisabledTexture, nullptr, &musicIconRect);
        SDL_RenderCopy(renderer, isSoundEnabled ? soundEnabledTexture : soundDisabledTexture, nullptr, &soundIconRect);
        SDL_RenderCopy(renderer, backTexture, nullptr, &settingRect);
        if(costume){
            SDL_SetTextureAlphaMod(settingCostumesTexture,128);
            SDL_SetTextureAlphaMod(settingMiscTexture,255);
        }
        else {
            SDL_SetTextureAlphaMod(settingMiscTexture,128);
            SDL_SetTextureAlphaMod(settingCostumesTexture,255);

        }
        SDL_RenderCopy(renderer, settingCostumesTexture, nullptr, &settingCostumesRect);
        SDL_RenderCopy(renderer, settingMiscTexture, nullptr, &settingMiscRect);
    }

    SDL_RenderPresent(renderer); // Hiển thị những gì đã vẽ
}

// Bắt đầu và duy trì vòng lặp chính của trò chơi
void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16); // Giới hạn FPS ~60
    }
}