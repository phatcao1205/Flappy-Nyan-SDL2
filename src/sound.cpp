// sound.cpp
#include "sound.h"
#include <iostream>

// Hàm khởi tạo Sound, khởi tạo SDL_mixer và tải các tệp âm thanh/nhạc
Sound::Sound() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    wingSound = Mix_LoadWAV("sounds/wing.wav");
    hitSound = Mix_LoadWAV("sounds/hit.wav");
    dieSound = Mix_LoadWAV("sounds/die.wav");
    pointSound = Mix_LoadWAV("sounds/point.wav");
    backgroundSound = Mix_LoadMUS("sounds/background.mp3");

}
// Hàm hủy, giải phóng tài nguyên âm thanh và đóng SDL_mixer
Sound::~Sound() {
    Mix_FreeChunk(wingSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(dieSound);
    Mix_FreeChunk(pointSound);
    Mix_FreeMusic(backgroundSound);
    Mix_CloseAudio();
    Mix_Quit(); // Gọi Mix_Quit khi kết thúc sử dụng SDL_mixer
}

// Phát âm thanh nhảy nếu soundEnabled là true
void Sound::playWingSound(bool soundEnabled) {
    if (soundEnabled && wingSound) {
        Mix_PlayChannel(-1, wingSound, 0);
    }
}

// Phát âm thanh va chạm nếu soundEnabled là true
void Sound::playHitSound(bool soundEnabled) {
    if (soundEnabled && hitSound) {
        Mix_PlayChannel(-1, hitSound, 0);
    }
}

// Phát âm thanh thua nếu soundEnabled là true
void Sound::playDieSound(bool soundEnabled) {
    if (soundEnabled && dieSound) {
        Mix_PlayChannel(-1, dieSound, 0);
    }
}

// Phát âm thanh ghi điểm nếu soundEnabled là true
void Sound::playPointSound(bool soundEnabled) {
    if (soundEnabled && pointSound) {
        Mix_PlayChannel(-1, pointSound, 0);
    }
}

// Phát nhạc nền lặp lại vô hạn (-1)
void Sound::playBackgroundMusic() {
    if (backgroundSound && !Mix_PlayingMusic()) { // Chỉ phát nếu có nhạc và chưa phát
        Mix_PlayMusic(backgroundSound, -1);
    }
}

// Dừng nhạc nền đang phát
void Sound::stopBackgroundMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
}