#include "text.h"

#include <SDL2/SDL_ttf.h>

static TTF_Font *font = NULL;

int init_text() {
    if (TTF_Init() == -1) {
        return -1;
    }

    font = TTF_OpenFont("ka1.ttf", 10);
    if (!font)
        return -1;

    return 0;
}

SDL_Surface *draw_text(const char *str, SDL_Color color) {
    return TTF_RenderText_Solid(font, str, color);
}

void cleanup_text() { TTF_Quit(); }
