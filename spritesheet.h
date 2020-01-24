#ifndef SPRITESHEET_H_RLEQFWAK
#define SPRITESHEET_H_RLEQFWAK

#include <SDL2/SDL.h>

typedef enum {
    EmptySprite = 0,
    SpaceshipSprite,
    InvaderSprite,
    Invader2Sprite,
} sprite_t;

int init_spritesheet(SDL_Renderer *r);
void draw_sprite(SDL_Renderer *r, sprite_t sprite, const SDL_Rect *pos);
unsigned spritew(sprite_t sprite);
unsigned spriteh(sprite_t sprite);
void cleanup_spritesheet();

#endif /* end of include guard: SPRITESHEET_H_RLEQFWAK */
