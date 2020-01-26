#ifndef UI_H_46PONAWE
#define UI_H_46PONAWE

#include <SDL2/SDL.h>

int init_ui();
void draw_ui(SDL_Renderer *r);
void cleanup_ui();

void add_score(unsigned amount);
void reset_score();

#endif /* end of include guard: UI_H_46PONAWE */
