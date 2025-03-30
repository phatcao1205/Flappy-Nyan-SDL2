// sound.cpp
#include "sound.h"
#include <iostream>

// Hàm khởi tạo âm thanh
Sound::Sound() {
    // Khởi tạo SDL Mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    // Tải các file âm thanh
    wingSound = Mix_LoadWAV("sounds/wing.wav");
    hitSound = Mix_LoadWAV("sounds/hit.wav");
    dieSound = Mix_LoadWAV("sounds/die.wav");
    pointSound = Mix_LoadWAV("sounds/point.wav");
    backgroundSound = Mix_LoadMUS("sounds/background.mp3");
}

// Hàm hủy, giải phóng tài nguyên âm thanh
Sound::~Sound() {
    Mix_FreeChunk(wingSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(dieSound);
    Mix_FreeChunk(pointSound);
    Mix_FreeMusic(backgroundSound);
    Mix_CloseAudio();
}

// Phát âm thanh khi chim nhảy
void Sound::playWingSound(bool sound) {
    if (sound) {
        Mix_PlayChannel(-1, wingSound, 0);
    }
}

// Phát âm thanh khi chim va chạm
void Sound::playHitSound(bool sound) {
    if (sound) {
        Mix_PlayChannel(-1, hitSound, 0);
    }
}

// Phát âm thanh khi chim thua
void Sound::playDieSound(bool sound) {
    if (sound) {
        Mix_PlayChannel(-1, dieSound, 0);
    }
}

// Phát âm thanh khi ghi điểm
void Sound::playPointSound(bool sound) {
    if (sound) {
        Mix_PlayChannel(-1, pointSound, 0);
    }
}
// Phát âm thanh khi ghi điểm
void Sound::playBackgroundMusic() {
    if (backgroundSound) {
        Mix_PlayMusic(backgroundSound, -1);
    }
}
// Dừng nhạc nền
void Sound::stopBackgroundMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
}
