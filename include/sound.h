// sound.h
#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>

// Lớp Sound quản lý các âm thanh trong trò chơi
class Sound {
public:
    Sound();   // Hàm khởi tạo
    ~Sound();  // Hàm hủy
    void playWingSound();   // Phát âm thanh khi chim nhảy
    void playHitSound();    // Phát âm thanh khi chim va chạm
    void playDieSound();    // Phát âm thanh khi chim thua
    void playPointSound();  // Phát âm thanh khi ghi điểm

private:
    Mix_Chunk* wingSound;   // Âm thanh khi chim nhảy
    Mix_Chunk* hitSound;    // Âm thanh khi va chạm
    Mix_Chunk* dieSound;    // Âm thanh khi thua
    Mix_Chunk* pointSound;  // Âm thanh khi ghi điểm
};

#endif