#include "game.h"


SDL_Texture* Game::loadTexture(const std::string& filePath) {
    SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

Game::Game() {
    window = SDL_CreateWindow("Flappy Nyan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* icon = IMG_Load("assets/icon.png");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    bird = new Bird();
    bird->rect = {100, (SCREEN_HEIGHT-80)/2, 80, 40};
    bird->velocity = 0;
    bird->angle = 0.0;
    running = true;
    gameState = MENU;
    delayStartTime = 0;
    scoreManager = new Score(renderer);

    srand(time(0));

    
    background = new Background(renderer);
    initPipes(); 
    pipeManager = new PipeManager(renderer);
    birdManager = new BirdManager(renderer);
    menu = new Menu(renderer);

    gameOverTexture = loadTexture("assets/gameover.png");
    gameOverRect = {(SCREEN_WIDTH - 384)/2, (SCREEN_HEIGHT - 84)/4, 192*2, 42*2};

    isSoundEnabled = true;
    soundEnabledTexture = loadTexture("assets/sound-enabled.png");
    soundDisabledTexture = loadTexture("assets/sound-disabled.png");
    soundIconRect = {SCREEN_WIDTH - 50 - 10, SCREEN_HEIGHT - 50 - 10, 50, 50};

    isBackgroundMusicEnabled = true;
    musicEnabledTexture = loadTexture("assets/music-enabled.png");
    musicDisabledTexture = loadTexture("assets/music-disabled.png");
    musicIconRect = {SCREEN_WIDTH - 50 - 50 - 20, SCREEN_HEIGHT - 50 - 10, 50, 50};

    sound.playBackgroundMusic(); 

    isInfoEnabled=false;
    infoTexture = loadTexture("assets/info.png");
    backTexture = loadTexture("assets/back.png");
    pauseTexture = loadTexture("assets/pause.png");
    infoRect = {10, SCREEN_HEIGHT - 50 - 10, 50, 50};

    tutTexture = loadTexture("assets/tut.png");
    tutRect = {20, 40, 775 , 400};
    
    pauseBoardTexture= loadTexture("assets/board.png");
    pauseBoardRect = {(SCREEN_WIDTH-500)/2, (SCREEN_HEIGHT-250)/2, 500, 250};

    resumeTexture= loadTexture("assets/resume.png");
    resumeRect = {(SCREEN_WIDTH-200)/2-120, (SCREEN_HEIGHT-60)/2+60, 200, 60};
    
    quitTexture= loadTexture("assets/quit.png");
    quitRect = {(SCREEN_WIDTH-200)/2+120, (SCREEN_HEIGHT-60)/2+60, 200, 60};
    
    currentTexture = loadTexture("assets/current.png");
    currentRect = {(SCREEN_WIDTH-300)/2-70, (SCREEN_HEIGHT-100)/2+100, 350, 60};

    highTexture = loadTexture("assets/high.png");
    highRect = {(SCREEN_WIDTH-300)/2-50, (SCREEN_HEIGHT-80)/2, 300, 80};

    restartTexture = loadTexture("assets/restart.png");
    restartRect = {(SCREEN_WIDTH-416)/2, (SCREEN_HEIGHT-50)/2+175, 416, 50};


}

Game::~Game() {
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(soundEnabledTexture);
    SDL_DestroyTexture(soundDisabledTexture);
    SDL_DestroyTexture(musicEnabledTexture);
    SDL_DestroyTexture(musicDisabledTexture);
    SDL_DestroyTexture(backTexture);
    SDL_DestroyTexture(pauseBoardTexture);
    SDL_DestroyTexture(resumeTexture);
    SDL_DestroyTexture(quitTexture);
    SDL_DestroyTexture(currentTexture);
    SDL_DestroyTexture(highTexture);
    SDL_DestroyTexture(restartTexture);

    delete bird;
    delete background;
    delete pipeManager;
    delete birdManager;
    delete menu;
    delete scoreManager;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

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

void Game::restart() {
    bird->rect = {100, SCREEN_HEIGHT / 2, 80, 40};
    bird->velocity = 0;
    bird->angle = 0.0;
    gameState = MENU;
    scoreManager->resetScore();
    delayStartTime = 0;
    initPipes();
    soundIconRect = {SCREEN_WIDTH - 50 - 10, SCREEN_HEIGHT - 50 - 10, 50, 50};
    musicIconRect = {SCREEN_WIDTH - 50 - 50 - 20, SCREEN_HEIGHT - 50 - 10, 50, 50};
}
bool Game::checkCollision() {
    int baseHeight = background->getBaseHeight();

    if (bird->rect.y + bird->rect.h > SCREEN_HEIGHT - baseHeight) {
        return true;
    }

    if (bird->rect.y < 0) {
        return true;
    }

    int currentPipeWidth = pipeManager->getPipeWidth();

    for (const auto& pipe : pipes) {
        SDL_Rect topPipeRect = { pipe.x, 0, currentPipeWidth, pipe.height };

        int bottomPipeY = pipe.height + PIPE_GAP;
        int bottomPipeHeight = SCREEN_HEIGHT - bottomPipeY;
        SDL_Rect bottomPipeRect = { pipe.x, bottomPipeY, currentPipeWidth, bottomPipeHeight };

        if (SDL_HasIntersection(&bird->rect, &topPipeRect)) {
            return true;
        }

        if (SDL_HasIntersection(&bird->rect, &bottomPipeRect)) {
            return true;
        }
    }

    return false;
}
void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            
            if (mouseX >= musicIconRect.x && mouseX <= musicIconRect.x + musicIconRect.w &&
                mouseY >= musicIconRect.y && mouseY <= musicIconRect.y + musicIconRect.h) {
                isBackgroundMusicEnabled = !isBackgroundMusicEnabled;
                if (isBackgroundMusicEnabled) sound.playBackgroundMusic();
                else sound.stopBackgroundMusic();
            }
            
            else if (mouseX >= soundIconRect.x && mouseX <= soundIconRect.x + soundIconRect.w &&
                     mouseY >= soundIconRect.y && mouseY <= soundIconRect.y + soundIconRect.h) {
                isSoundEnabled = !isSoundEnabled;
                sound.playPointSound(isSoundEnabled);
            }
            else if (mouseX >= infoRect.x && mouseX <= infoRect.x + infoRect.w &&
                     mouseY >= infoRect.y && mouseY <= infoRect.y + infoRect.h) {
                    isInfoEnabled = !isInfoEnabled;
                    if(isInfoEnabled&&gameState==MENU) gameState=INFO;
                    else if(gameState==PLAYING) gameState=PAUSE;
                    else if(gameState==INFO) gameState=MENU;
            }
            else if (gameState == MENU) {
                gameState = STARTING;
                delayStartTime = SDL_GetTicks();
                sound.playWingSound(isSoundEnabled);
            } else if (gameState == PLAYING) {
                bird->velocity = JUMP_STRENGTH;
                sound.playWingSound(isSoundEnabled);
            } else if (gameState == PAUSE){
                if (mouseX >= resumeRect.x && mouseX <= resumeRect.x + resumeRect.w &&
                    mouseY >= resumeRect.y && mouseY <= resumeRect.y + resumeRect.h) {
                       gameState = RESUMING;
                       delayStartTime = SDL_GetTicks();
                    }
                else if (mouseX >= quitRect.x && mouseX <= quitRect.x + quitRect.w &&
                    mouseY >= quitRect.y && mouseY <= quitRect.y + quitRect.h) {
                       restart();
                    }
            }
       }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                if (gameState == MENU) {
                    gameState = STARTING; 
                    delayStartTime = SDL_GetTicks();
                    sound.playWingSound(isSoundEnabled);
                } else if (gameState == PLAYING) {
                    bird->velocity = JUMP_STRENGTH;
                    sound.playWingSound(isSoundEnabled);
                }
            }
            else if (event.key.keysym.sym == SDLK_r && gameState == GAME_OVER) {
                restart();
            }
            else if (event.key.keysym.sym == SDLK_ESCAPE){
                if(gameState==PLAYING) gameState=PAUSE;
                else if(gameState==PAUSE) gameState=PLAYING;
            }
        }
    }
}


void Game::update() {
    if (gameState != GAME_OVER && gameState!=PAUSE && gameState!=RESUMING) {
        background->update(); 
    }

    if (gameState == MENU || gameState == INFO) {
        birdManager->updateBird(*bird, gameState); 
        return;
    }

    if (gameState == STARTING) {
        birdManager->updateBird(*bird, MENU); 
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - delayStartTime >= 3000) {
            gameState = PLAYING; 
        }
        return;

    }
    if (gameState == RESUMING) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - delayStartTime >= 3000) {
            gameState = PLAYING; 
        }
        return;
    }


    if (gameState == GAME_OVER|| gameState == PAUSE) {
        return; 
    }
    
    birdManager->updateBird(*bird, gameState); 

    int baseHeight = background->getBaseHeight();
    
    for (size_t i = 0; i < pipes.size(); i++) {
        pipes[i].x -= PIPE_SPEED;
        
        if (!pipePassed[i] && bird->rect.x > pipes[i].x + PIPE_WIDTH) {
            pipePassed[i] = true;
            scoreManager->incrementScore();
            sound.playPointSound(isSoundEnabled);
        }

        
        if (pipes[i].x + PIPE_WIDTH < 0) {
            pipes[i].x = SCREEN_WIDTH + (pipes.size() > 1 ? 0 : 150); 
            int maxPipeHeight = SCREEN_HEIGHT - baseHeight - PIPE_GAP - MIN_BOTTOM_PIPE_HEIGHT - 50;
            pipes[i].height = rand() % (maxPipeHeight - 50 + 1) + 50;
            pipePassed[i] = false;
        }
    }

    
    if (checkCollision()) {
        gameState = GAME_OVER;
        sound.playHitSound(isSoundEnabled);
        sound.playDieSound(isSoundEnabled);
        scoreManager->checkHighScore();
    }
}

void Game::render() {
    background->render(); 
    birdManager->render(renderer, *bird); 

    int baseHeight = background->getBaseHeight();

    if (gameState == MENU) {
        menu->render(); 
        
        SDL_RenderCopy(renderer, infoTexture, nullptr, &infoRect);
        SDL_RenderCopy(renderer, isBackgroundMusicEnabled ? musicEnabledTexture : musicDisabledTexture, nullptr, &musicIconRect);
        SDL_RenderCopy(renderer, isSoundEnabled ? soundEnabledTexture : soundDisabledTexture, nullptr, &soundIconRect);
    } else if (gameState == STARTING || gameState == PLAYING || gameState == RESUMING) {
        
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - delayStartTime <= 1000) {
            scoreManager->renderTimer(3);
        }
        else if (currentTime - delayStartTime <= 2000) {
            scoreManager->renderTimer(2);
        }
        else if (currentTime - delayStartTime <= 3000) {
            scoreManager->renderTimer(1);
        }
        for (const auto& pipe : pipes) {
            pipeManager->render(renderer, pipe, baseHeight);
        }
        scoreManager->render(gameState);
        
        SDL_RenderCopy(renderer, pauseTexture, nullptr, &infoRect);
    } else if (gameState == GAME_OVER) {
        
        for (const auto& pipe : pipes) {
            pipeManager->render(renderer, pipe, baseHeight);
        }
        scoreManager->render(gameState);
            SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
            SDL_RenderCopy(renderer, currentTexture, nullptr, &currentRect);
            SDL_RenderCopy(renderer, highTexture, nullptr, &highRect);
            SDL_RenderCopy(renderer, restartTexture, nullptr, &restartRect);
    }
    else if (gameState == PAUSE){
        for (const auto& pipe : pipes) {
            pipeManager->render(renderer, pipe, baseHeight);
        }
        SDL_RenderCopy(renderer, pauseBoardTexture, nullptr, &pauseBoardRect);
        SDL_RenderCopy(renderer, resumeTexture, nullptr, &resumeRect);
        SDL_RenderCopy(renderer, quitTexture, nullptr, &quitRect);
        musicIconRect = {(SCREEN_WIDTH-80)/2-80, (SCREEN_HEIGHT-80)/2-30, 80, 80};
        soundIconRect = {(SCREEN_WIDTH-80)/2+80, (SCREEN_HEIGHT-80)/2-30, 80, 80};
        SDL_RenderCopy(renderer, isBackgroundMusicEnabled ? musicEnabledTexture : musicDisabledTexture, nullptr, &musicIconRect);
        SDL_RenderCopy(renderer, isSoundEnabled ? soundEnabledTexture : soundDisabledTexture, nullptr, &soundIconRect);
    }
    else if (gameState == INFO){
        SDL_RenderCopy(renderer, backTexture, nullptr, &infoRect);
        SDL_RenderCopy(renderer, tutTexture, nullptr, &tutRect);
        SDL_RenderCopy(renderer, isBackgroundMusicEnabled ? musicEnabledTexture : musicDisabledTexture, nullptr, &musicIconRect);
        SDL_RenderCopy(renderer, isSoundEnabled ? soundEnabledTexture : soundDisabledTexture, nullptr, &soundIconRect);
    }
    SDL_RenderPresent(renderer); 
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16); 
    }
}