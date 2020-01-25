#include "ui.h"

#include <SDL2/SDL.h>

#include "spritesheet.h"
#include "text.h"

static const SDL_Rect uiRect = {0, 0, 800, 50};
static const SDL_Rect uiInvadersShotRect = {50, 10, 30, 20};

static SDL_Rect scoreTextureOnScreen = {100, 10, 16, 16};
static SDL_Rect scoreTextureRect = {0, 0, 0, 0};

static unsigned score = 0;

static SDL_Texture *scoreTexture = NULL;
static const SDL_Color textColor = {255, 255, 255, 255};
static SDL_bool needRedraw = SDL_TRUE;

static void redraw_score(SDL_Renderer *r) {
    char buf[16];

    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = NULL;
    }

    snprintf(buf, 16, "score %u", score);

    SDL_Surface *scoreSurface = draw_text(buf, textColor);
    scoreTextureRect.w = scoreSurface->w;
    scoreTextureRect.h = scoreSurface->h;
    scoreTextureOnScreen.w = scoreTextureRect.w;
    scoreTextureOnScreen.h = scoreTextureRect.h;
    scoreTexture = SDL_CreateTextureFromSurface(r, scoreSurface);
    SDL_FreeSurface(scoreSurface);
    needRedraw = SDL_FALSE;
}

int init_ui() {}

void draw_ui(SDL_Renderer *r) {
    if (needRedraw) {
        redraw_score(r);
    }

    SDL_SetRenderDrawColor(r, 40, 40, 40, 255);
    SDL_RenderFillRect(r, &uiRect);

    /* draw_sprite(r, InvaderSprite, &uiInvadersShotRect); */
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);

    SDL_RenderCopy(r, scoreTexture, &scoreTextureRect, &scoreTextureOnScreen);
}

void add_score(unsigned amount) {
    score += amount;
    needRedraw = SDL_TRUE;
}

void cleanup_ui() {
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = NULL;
    }
}

