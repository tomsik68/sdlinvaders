#include "spritesheet.h"

#include <SDL2/SDL_image.h>

static SDL_Texture *spritesheet = NULL;

#define SPRITEW 34
#define SPRITEH 22

#define SPRITE_AT(col, row) \
    { SPRITEW * col, SPRITEH * row, SPRITEW, SPRITEH }

#define INVADER_AT(col, row) \
    SPRITE_AT(col, row), SPRITE_AT((col + 1), row)

static const SDL_Rect sprites[] = {
    {0, 0, 0, 0},      SPRITE_AT(12, 14), INVADER_AT(16, 0), INVADER_AT(16, 1),
    INVADER_AT(16, 2), INVADER_AT(16, 3), INVADER_AT(16, 4), INVADER_AT(16, 5),
    INVADER_AT(16, 6), INVADER_AT(16, 7), INVADER_AT(16, 8), INVADER_AT(16, 9),
    INVADER_AT(16, 10)};

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

unsigned spritew(sprite_t sprite) { return sprites[sprite].w; }

unsigned spriteh(sprite_t sprite) { return sprites[sprite].h; }

void cleanup_spritesheet() { SDL_DestroyTexture(spritesheet); }

