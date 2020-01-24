#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"

#define MS_IN_FRAME (1000 / 60)

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        puts(SDL_GetError());
        goto init_fail;
    }

    SDL_Window *wnd =
        SDL_CreateWindow("test", 0, 0, 800, 600, SDL_WINDOW_BORDERLESS);
    if (!wnd) {
        goto window_fail;
    }

    SDL_Renderer *r = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    if (!r) {
        goto renderer_fail;
    }

    init_game(r);

    unsigned lastTime = 0, currentTime = 0, elapsed;
    for (;;) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }

            handle_event(&event);
        }

        SDL_RenderClear(r);
        draw_game(r);
        SDL_RenderPresent(r);

        currentTime = SDL_GetTicks();
        elapsed = currentTime - lastTime;
        if (elapsed < MS_IN_FRAME) {
            SDL_Delay(MS_IN_FRAME - elapsed);
        }
    }

    cleanup_game();
    SDL_DestroyRenderer(r);
renderer_fail:
    SDL_DestroyWindow(wnd);
window_fail:
    SDL_Quit();
init_fail:

    return 0;
}
