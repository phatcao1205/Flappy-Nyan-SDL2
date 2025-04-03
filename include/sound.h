// sound.h
#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>

// Lớp Sound quản lý việc tải và phát âm thanh, nhạc nền
class Sound {
public:
    // Hàm khởi tạo Sound
    Sound();
    // Hàm hủy Sound
    ~Sound();
    // Phát âm thanh nhảy (nếu bật)
    void playWingSound(bool soundEnabled);
    // Phát âm thanh va chạm (nếu bật)
    void playHitSound(bool soundEnabled);
    // Phát âm thanh thua (nếu bật)
    void playDieSound(bool soundEnabled);
    // Phát âm thanh ghi điểm (nếu bật)
    void playPointSound(bool soundEnabled);
    // Phát nhạc nền lặp lại
    void playBackgroundMusic();
    // Dừng phát nhạc nền
    void stopBackgroundMusic();

private:
    Mix_Chunk* wingSound;
    Mix_Chunk* hitSound;
    Mix_Chunk* dieSound;
    Mix_Chunk* pointSound;
    Mix_Music* backgroundSound;
    // Biến 'sound' trong header gốc không được sử dụng và bị che bởi tham số, nên đã loại bỏ
};

#endif // SOUND_H