#ifndef SOUND_H_9J38RYXZ
#define SOUND_H_9J38RYXZ

typedef enum {
    Laser,
    Beep,
    Hit,
} sound_t;

void init_sound();
void play_sound(sound_t snd);
void cleanup_sound();


#endif /* end of include guard: SOUND_H_9J38RYXZ */
