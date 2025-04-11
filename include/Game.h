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
#include "score.h"

struct Bird;
class BirdManager;

class Game {
public:
    Game();
    ~Game();
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
    Background* background;
    PipeManager* pipeManager;
    BirdManager* birdManager;
    Menu* menu;
    Score* scoreManager;

    Uint32 delayStartTime;

    SDL_Texture* gameOverTexture;
    SDL_Rect gameOverRect;

    SDL_Texture* musicEnabledTexture;
    SDL_Texture* musicDisabledTexture;
    SDL_Rect musicIconRect;

    SDL_Texture* soundEnabledTexture;
    SDL_Texture* soundDisabledTexture;
    SDL_Rect soundIconRect;

    SDL_Texture* infoTexture;
    SDL_Texture* backTexture;
    SDL_Texture* pauseTexture;
    SDL_Rect infoRect;

    SDL_Texture* tutTexture;
    SDL_Rect tutRect;      
    
    SDL_Texture* pauseBoardTexture; 
    SDL_Rect pauseBoardRect;      
    
    SDL_Texture* resumeTexture; 
    SDL_Rect resumeRect;     

    SDL_Texture* quitTexture; 
    SDL_Rect quitRect;  

    SDL_Texture* currentTexture; 
    SDL_Rect currentRect; 
    
    SDL_Texture* highTexture; 
    SDL_Rect highRect; 

    SDL_Texture* restartTexture; 
    SDL_Rect restartRect; 


    bool isInfoEnabled;
    bool isBackgroundMusicEnabled;
    bool isSoundEnabled;

    SDL_Texture* loadTexture(const std::string& filePath);
    void handleEvents();
    void update();
    void render();
    void initPipes();
    void restart();
    bool checkCollision();
    
};

#endif