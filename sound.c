
#include "sound.h"

#include <SDL2/SDL_mixer.h>

static Mix_Chunk *sounds[3] = {0};

void init_sound() {
    Mix_Init(0);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_AllocateChannels(1);
    Mix_Volume(-1, MIX_MAX_VOLUME);

    sounds[Laser] = Mix_LoadWAV("shot.wav");
    sounds[Beep] = Mix_LoadWAV("beep.wav");
    sounds[Hit] = Mix_LoadWAV("hit.wav");
}

void play_sound(sound_t snd) {
    Mix_PlayChannel(0, sounds[snd], 0);
}

void cleanup_sound() {
    for (size_t i = 0; i < sizeof(sounds); ++i) {
        Mix_FreeChunk(sounds[i]);
    }

    Mix_Quit();
}

