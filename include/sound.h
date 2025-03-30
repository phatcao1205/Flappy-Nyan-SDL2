// sound.h
#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>

// Lớp Sound quản lý các âm thanh trong trò chơi
class Sound {
public:
    Sound();   // Hàm khởi tạo
    ~Sound();  // Hàm hủy
    void playWingSound(bool sound);   // Phát âm thanh khi chim nhảy
    void playHitSound(bool sound);    // Phát âm thanh khi chim va chạm
    void playDieSound(bool sound);    // Phát âm thanh khi chim thua
    void playPointSound(bool sound);  // Phát âm thanh khi ghi điểm
    void playBackgroundMusic(); // Phát âm thanh khi ở ngoài menu
    void stopBackgroundMusic();  // Dừng nhạc nền

private:
    Mix_Chunk* wingSound;   // Âm thanh khi chim nhảy
    Mix_Chunk* hitSound;    // Âm thanh khi va chạm
    Mix_Chunk* dieSound;    // Âm thanh khi thua
    Mix_Chunk* pointSound;  // Âm thanh khi ghi điểm
    Mix_Music* backgroundSound;  // Âm thanh khi ở ngoài menu
    bool sound;
};

#endif