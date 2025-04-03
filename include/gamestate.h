// gamestate.h
#ifndef GAMESTATE_H
#define GAMESTATE_H

// Định nghĩa các trạng thái có thể có của trò chơi
enum GameState {
    MENU,      // Trạng thái hiển thị menu chính
    STARTING,  // Trạng thái chờ ngắn trước khi bắt đầu chơi
    PLAYING,   // Trạng thái đang chơi
    GAME_OVER, // Trạng thái kết thúc trò chơi
    SETTING    // Trạng thái màn hình cài đặt
};

#endif // GAMESTATE_H