#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>

class Sound {
public:
    Sound();
    ~Sound();
    void playWingSound(bool soundEnabled);
    void playHitSound(bool soundEnabled);
    void playDieSound(bool soundEnabled);
    void playPointSound(bool soundEnabled);
    void playBackgroundMusic();
    void stopBackgroundMusic();

private:
    Mix_Chunk* wingSound;
    Mix_Chunk* hitSound;
    Mix_Chunk* dieSound;
    Mix_Chunk* pointSound;
    Mix_Music* backgroundSound;
    
};

#endif