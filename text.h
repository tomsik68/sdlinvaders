#ifndef TEXT_H_UJZEXOMO
#define TEXT_H_UJZEXOMO

#include <SDL2/SDL_surface.h>

int init_text();

SDL_Surface *draw_text(const char *str, SDL_Color color);

void cleanup_text();


#endif /* end of include guard: TEXT_H_UJZEXOMO */
