#include "sound.h"
#include <iostream>

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
Sound::~Sound() {
    Mix_FreeChunk(wingSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(dieSound);
    Mix_FreeChunk(pointSound);
    Mix_FreeMusic(backgroundSound);
    Mix_CloseAudio();
    Mix_Quit(); 
}

void Sound::playWingSound(bool soundEnabled) {
    if (soundEnabled && wingSound) {
        Mix_PlayChannel(-1, wingSound, 0);
    }
}

void Sound::playHitSound(bool soundEnabled) {
    if (soundEnabled && hitSound) {
        Mix_PlayChannel(-1, hitSound, 0);
    }
}

void Sound::playDieSound(bool soundEnabled) {
    if (soundEnabled && dieSound) {
        Mix_PlayChannel(-1, dieSound, 0);
    }
}

void Sound::playPointSound(bool soundEnabled) {
    if (soundEnabled && pointSound) {
        Mix_PlayChannel(-1, pointSound, 0);
    }
}

void Sound::playBackgroundMusic() {
    if (backgroundSound && !Mix_PlayingMusic()) { 
        Mix_PlayMusic(backgroundSound, -1);
    }
}

void Sound::stopBackgroundMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
}