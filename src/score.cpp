#include "score.h"


// Hàm khởi tạo
Score::Score(SDL_Renderer* ren) : renderer(ren), score(0) { // <<--- ĐỔI TÊN CLASS::CONSTRUCTOR
    loadDigitTextures();
    highScore = loadScoreFromFile();
}

// Hàm hủy
Score::~Score() { 
    for (int i = 0; i < 10; ++i) {
        if (digitTextures[i]) {
            SDL_DestroyTexture(digitTextures[i]);
            digitTextures[i] = nullptr;
        }
    }
}

// Tải một texture từ đường dẫn file
SDL_Texture* Score::loadTexture(const std::string& filePath) { 
    SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}


// Tải các texture cho chữ số từ 0 đến 9
void Score::loadDigitTextures() {
    for (int i = 0; i < 10; ++i) {
        std::string path = "assets/" + std::to_string(i) + ".png";
        digitTextures[i] = loadTexture(path);
    }
}

// Tăng điểm
void Score::incrementScore() {
    score++;
}

// Reset điểm
void Score::resetScore() {
    score = 0;
}

// Vẽ điểm số hiện tại lên màn hình sử dụng các texture chữ số
void Score::render(GameState gameState) {
    std::string scoreStr = std::to_string(score);
    std::string highScoreStr = std::to_string(highScore);
    int digitWidth = 30;
    int digitHeight = 50;
    int totalWidth = scoreStr.length() * digitWidth;
    int scoreX = (SCREEN_WIDTH - totalWidth) / 2;
    int scoreY = 50;
    if(gameState == GAME_OVER) {
        scoreX = (SCREEN_WIDTH-300)/2+290;
        scoreY = (SCREEN_HEIGHT-100)/2+110;
        int highScoreX=(SCREEN_WIDTH-300)/2+260;
        int highScoreY=(SCREEN_HEIGHT-80)/2+15;
        for (size_t i = 0; i < highScoreStr.length(); ++i) {
            int digit = highScoreStr[i] - '0';
                SDL_Rect digitRect = {highScoreX + static_cast<int>(i * digitWidth), highScoreY, digitWidth, digitHeight};
                SDL_RenderCopy(renderer, digitTextures[digit], nullptr, &digitRect);
        }
    }
    for (size_t i = 0; i < scoreStr.length(); ++i) {
        int digit = scoreStr[i] - '0';
            SDL_Rect digitRect = {scoreX + static_cast<int>(i * digitWidth), scoreY, digitWidth, digitHeight};
            SDL_RenderCopy(renderer, digitTextures[digit], nullptr, &digitRect);
    }
}

int Score::loadScoreFromFile() {
    std::ifstream file("include/highScore.txt");
    int score;
    file >> score;
    file.close();
    return score;
}
void Score::saveScore(int score){
    std::ofstream file("include/highScore.txt");
    if (file.is_open()) {
        file << score;
        file.close();
    }
}
void Score::checkHighScore(){
    if(score>highScore){
        highScore=score;
        saveScore(highScore);
    }
}