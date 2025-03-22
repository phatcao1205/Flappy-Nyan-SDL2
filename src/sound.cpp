// sound.cpp
#include "sound.h"
#include <iostream>

// Hàm khởi tạo âm thanh
Sound::Sound() {
    // Khởi tạo SDL Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Không thể khởi tạo SDL Mixer: " << Mix_GetError() << std::endl;
    }

    // Tải các file âm thanh
    wingSound = Mix_LoadWAV("sounds/wing.wav");
    hitSound = Mix_LoadWAV("sounds/hit.wav");
    dieSound = Mix_LoadWAV("sounds/die.wav");
    pointSound = Mix_LoadWAV("sounds/point.wav");

    // Kiểm tra nếu có lỗi khi tải âm thanh
    if (!wingSound || !hitSound || !dieSound || !pointSound) {
        std::cerr << "Không thể tải âm thanh: " << Mix_GetError() << std::endl;
    }
}

// Hàm hủy, giải phóng tài nguyên âm thanh
Sound::~Sound() {
    Mix_FreeChunk(wingSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(dieSound);
    Mix_FreeChunk(pointSound);
    Mix_CloseAudio();
}

// Phát âm thanh khi chim nhảy
void Sound::playWingSound() {
    if (wingSound) {
        Mix_PlayChannel(-1, wingSound, 0);
    }
}

// Phát âm thanh khi chim va chạm
void Sound::playHitSound() {
    if (hitSound) {
        Mix_PlayChannel(-1, hitSound, 0);
    }
}

// Phát âm thanh khi chim thua
void Sound::playDieSound() {
    if (dieSound) {
        Mix_PlayChannel(-1, dieSound, 0);
    }
}

// Phát âm thanh khi ghi điểm
void Sound::playPointSound() {
    if (pointSound) {
        Mix_PlayChannel(-1, pointSound, 0);
    }
}