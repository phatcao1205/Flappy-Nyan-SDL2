#ifndef SCORE_H    
#define SCORE_H    

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <fstream>
#include "constants.h"
#include "gamestate.h"

class Score {
public:
    Score(SDL_Renderer* renderer); 
    ~Score();

    void incrementScore();
    void resetScore();
    void render(GameState gameState);
    void renderTimer(int n);
    void checkHighScore();

private:
    SDL_Renderer* renderer;
    int score;
    int highScore;
    SDL_Texture* digitTextures[10];

    SDL_Texture* loadTexture(const std::string& filePath);
    void loadDigitTextures();
    int loadScoreFromFile();
    void saveScore(int score);
};

#endif