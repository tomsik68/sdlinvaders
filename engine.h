#ifndef ENGINE_H_ZJJSNTEA
#define ENGINE_H_ZJJSNTEA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *png2tex(SDL_Renderer *r, const char *path) {
    SDL_Surface *s = IMG_Load(path);
    if (!s) {
        // TODO
        return NULL;
    }

    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);

    SDL_FreeSurface(s);
    // if t == NULL, NULL is returned. all good
    return t;
}

#endif /* end of include guard: ENGINE_H_ZJJSNTEA */
