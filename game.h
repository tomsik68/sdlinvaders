#ifndef GAME_H_LBKAX7QS
#define GAME_H_LBKAX7QS

#include <SDL2/SDL.h>

void init_game(SDL_Renderer *r);
void handle_event(SDL_Event *event);
void draw_game(SDL_Renderer *r);
void cleanup_game();

#endif /* end of include guard: GAME_H_LBKAX7QS */
