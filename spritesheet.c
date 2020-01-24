#include "spritesheet.h"

#include <SDL2/SDL_image.h>

static SDL_Texture *spritesheet = NULL;

static const SDL_Rect sprites[] = {
    {0, 0, 0, 0}, {410, 312, 32, 18}, {548, 24, 30, 20}, {582, 24, 30, 20}};

int init_spritesheet(SDL_Renderer *r) {
    SDL_Surface *s = IMG_Load("./invader.png");
    if (!s) {
        return 0;
    }

    SDL_SetColorKey(s, SDL_TRUE, 0);

    spritesheet = SDL_CreateTextureFromSurface(r, s);

    SDL_FreeSurface(s);

    if (spritesheet) return 1;
    return 0;
}

void draw_sprite(SDL_Renderer *r, sprite_t sprite, const SDL_Rect *pos) {
    SDL_RenderCopy(r, spritesheet, &sprites[sprite], pos);
}

unsigned spritew(sprite_t sprite) {
    return sprites[sprite].w; }

unsigned spriteh(sprite_t sprite) { return sprites[sprite].h; }

void cleanup_spritesheet() { SDL_DestroyTexture(spritesheet); }

