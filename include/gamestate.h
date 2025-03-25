// gamestate.h
#ifndef GAMESTATE_H
#define GAMESTATE_H

// Trạng thái của trò chơi
enum GameState {
    MENU,      // Trạng thái hiển thị menu
    STARTING,  // Trạng thái delay sau khi nhấn Space (mới thêm)
    PLAYING,   // Trạng thái đang chơi
    GAME_OVER  // Trạng thái kết thúc trò chơi
};

#endif