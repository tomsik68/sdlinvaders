#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

#define NINVADERS 64
#define NMAXSHOT 8
#define NINVADER_TYPES 11

#include "spritesheet.h"
#include "text.h"
#include "ui.h"
#include "sound.h"

static SDL_Rect ship;
static SDL_Rect shots[NMAXSHOT];

static unsigned last_move;
static unsigned last_inv_move;
static unsigned last_shot;

static SDL_Rect inv;
static sprite_t invader_type[NINVADERS];

static SDL_Rect screenRect = {0, 0, WIDTH, HEIGHT};
static SDL_Texture *background = NULL;

static SDL_Texture *startGameText = NULL;
static SDL_Rect startGameTextRect = {0, 0, 0, 0};
static SDL_Rect startGameTextRectOnScreen = {0, 0, 0, 0};

static SDL_Texture *pausedGameText = NULL;
static SDL_Rect pausedGameTextRect = {0, 0, 0, 0};
static SDL_Rect pausedGameTextRectOnScreen = {0, 0, 0, 0};

typedef enum {
    Menu,
    Game,
    Pause,
} state_t;

static state_t state = Menu;

static void place_ship() {
    ship.x = WIDTH / 2 - spritew(SpaceshipSprite) / 2;
    ship.y = HEIGHT - spriteh(SpaceshipSprite);
    ship.w = spritew(SpaceshipSprite);
    ship.h = spriteh(SpaceshipSprite);
}

static void init_invaders() {
    inv.x = -spritew(InvaderSprite);
    inv.y = 100;
    inv.w = spritew(InvaderSprite);
    inv.h = spriteh(InvaderSprite);

    for (size_t i = 0; i < NINVADERS; ++i) {
        invader_type[i] = InvaderSprite + 2 * (rand() % NINVADER_TYPES);
    }
}

static void init_shots() {
    for (size_t i = 0; i < NMAXSHOT; ++i) {
        shots[i].x = -999;
        shots[i].y = -999;
        shots[i].w = 4;
        shots[i].h = 8;
    }
}

static void load_background(SDL_Renderer *r) {
    SDL_Surface *bg = IMG_Load("./background.png");
    background = SDL_CreateTextureFromSurface(r, bg);
    SDL_FreeSurface(bg);
}

static void init_start_game_text(SDL_Renderer *r) {
    static SDL_Color startGameTextColor = {255, 255, 255, 255};
    SDL_Surface *startGameSurface =
        draw_text("press <enter> to play", startGameTextColor);
    startGameTextRect.w = startGameSurface->w;
    startGameTextRect.h = startGameSurface->h;
    startGameTextRectOnScreen.w = startGameSurface->w;
    startGameTextRectOnScreen.h = startGameSurface->h;
    startGameTextRectOnScreen.x = WIDTH / 2 - startGameTextRect.w / 2;
    startGameTextRectOnScreen.y = HEIGHT / 2 - startGameTextRect.h / 2;
    startGameText = SDL_CreateTextureFromSurface(r, startGameSurface);
    SDL_FreeSurface(startGameSurface);

    SDL_Surface *pausedGameSurface =
        draw_text("press P to resume", startGameTextColor);
    pausedGameTextRect.w = pausedGameSurface->w;
    pausedGameTextRect.h = pausedGameSurface->h;
    pausedGameTextRectOnScreen.w = pausedGameSurface->w;
    pausedGameTextRectOnScreen.h = pausedGameSurface->h;
    pausedGameTextRectOnScreen.x = WIDTH / 2 - pausedGameTextRect.w / 2;
    pausedGameTextRectOnScreen.y = HEIGHT / 2 - pausedGameTextRect.h / 2;
    pausedGameText = SDL_CreateTextureFromSurface(r, pausedGameSurface);
    SDL_FreeSurface(pausedGameSurface);
}

void init_game(SDL_Renderer *r) {
    srand(time(NULL));
    load_background(r);
    init_start_game_text(r);
    init_spritesheet(r);
    place_ship();
    init_invaders();
    init_shots();
    init_ui();

    last_move = 0;
    last_inv_move = 0;
    last_shot = 0;
    state = Menu;
}

static void transition_menu_to_game() {
    state = Game;
    place_ship();
    init_invaders();
    init_shots();
}

static void transition_game_to_pause() {
    state = Pause;
}

static void transition_pause_to_game() {
    state = Game;
}

void handle_event(SDL_Event *event) {
    SDL_KeyboardEvent *ke = (SDL_KeyboardEvent *)event;
    if (state == Menu) {
        if (event->type == SDL_KEYDOWN && !ke->repeat &&
            ke->keysym.scancode == SDL_SCANCODE_RETURN) {
			play_sound(Beep);
            transition_menu_to_game();
        }
    } else if (state == Game) {
        if (event->type == SDL_KEYDOWN && !ke->repeat &&
            ke->keysym.scancode == SDL_SCANCODE_P) {
			play_sound(Beep);
            transition_game_to_pause();
        }
    } else if (state == Pause) {
        if (event->type == SDL_KEYDOWN && !ke->repeat &&
            ke->keysym.scancode == SDL_SCANCODE_P) {
			play_sound(Beep);
            transition_pause_to_game();
        }
    }
}

static void compute_invader_pos(size_t i, SDL_Rect *pos) {
    sprite_t s = invader_type[i];
    pos->x = inv.x + (spritew(InvaderSprite) + 4) * i;
    pos->y = inv.y + (pos->x / (WIDTH + 2 * spritew(InvaderSprite))) *
                         (spriteh(InvaderSprite) + 4);
    pos->x %= WIDTH + 2 * spritew(InvaderSprite);
    pos->x -= spritew(InvaderSprite);
    pos->w = spritew(s);
    pos->h = spriteh(s);
}

static void draw_invaders(SDL_Renderer *r) {
    sprite_t s = InvaderSprite;

    SDL_Rect pos = inv;
    for (size_t i = 0; i < NINVADERS; ++i) {
        if (invader_type[i] != EmptySprite) {
            s = invader_type[i];
            compute_invader_pos(i, &pos);

            if (SDL_GetTicks() % 1000 > 500) {
                ++s;
            }

            draw_sprite(r, s, &pos);
        }
    }
}

static void shoot() {
    if (SDL_GetTicks() - last_shot > 200) {
        for (size_t i = 0; i < NMAXSHOT; ++i) {
            if (shots[i].x == -999) {
                shots[i].x = ship.x + spritew(SpaceshipSprite) / 2;
                shots[i].y = ship.y - 4;
				play_sound(Laser);
                break;
            }
        }

        last_shot = SDL_GetTicks();
    }
}

static void move_ship() {
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    if (keyboard[SDL_SCANCODE_SPACE]) {
        shoot();
    }

    if (SDL_GetTicks() - last_move > 20) {
        if (keyboard[SDL_SCANCODE_RIGHT]) {
            ship.x += 4;
        }
        if (keyboard[SDL_SCANCODE_LEFT]) {
            ship.x -= 4;
        }

        last_move = SDL_GetTicks();
    }
}

static void move_shots() {
    SDL_Rect dummy;
    SDL_Rect invader;

    for (size_t i = 0; i < NMAXSHOT; ++i) {
        shots[i].y -= 2;

        if (shots[i].y < -10) {
            shots[i].x = -999;
            shots[i].y = -999;
        }

        if (shots[i].x != -999) {
            for (size_t j = 0; j < NINVADERS; ++j) {
                compute_invader_pos(j, &invader);
                if (SDL_IntersectRect(&shots[i], &invader, &dummy) ==
                    SDL_TRUE) {
                    invader_type[j] = EmptySprite;
                    shots[i].x = -999;
                    add_score(10);
					play_sound(Hit);
                    break;
                }
            }
        }
    }
}

static void move_invaders() {
    if (SDL_GetTicks() - last_inv_move > 15) {
        inv.x += 1;
        last_inv_move = SDL_GetTicks();

        if (inv.x == WIDTH + spritew(InvaderSprite)) {
            inv.x = -spritew(InvaderSprite);
            inv.y += spriteh(InvaderSprite) + 3;
        }

        move_shots();
    }
}

static void draw_shots(SDL_Renderer *r) {
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);

    for (size_t i = 0; i < NMAXSHOT; ++i) {
        SDL_RenderFillRect(r, &shots[i]);
    }

    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
}

static void draw_menu(SDL_Renderer *r) {
    SDL_RenderCopy(r, background, &screenRect, &screenRect);
    draw_sprite(r, SpaceshipSprite, &ship);

    draw_invaders(r);
    draw_shots(r);

    draw_ui(r);

    SDL_SetRenderDrawColor(r, 0, 0, 0, 150);
    SDL_RenderFillRect(r, &screenRect);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);

    if (SDL_GetTicks() % 2000 > 500) {
        SDL_RenderCopy(r, startGameText, &startGameTextRect,
                       &startGameTextRectOnScreen);
    }
}

static void draw_pause(SDL_Renderer *r) {
    SDL_RenderCopy(r, background, &screenRect, &screenRect);
    draw_sprite(r, SpaceshipSprite, &ship);

    draw_invaders(r);
    draw_shots(r);

    draw_ui(r);

    SDL_SetRenderDrawColor(r, 0, 0, 0, 150);
    SDL_RenderFillRect(r, &screenRect);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);

    if (SDL_GetTicks() % 2000 > 500) {
        SDL_RenderCopy(r, pausedGameText, &pausedGameTextRect,
                       &pausedGameTextRectOnScreen);
    }
}

static void draw_ingame(SDL_Renderer *r) {
    SDL_RenderCopy(r, background, &screenRect, &screenRect);
    draw_sprite(r, SpaceshipSprite, &ship);
    draw_invaders(r);
    draw_shots(r);

    move_ship();
    move_invaders();

    draw_ui(r);
}

void draw_game(SDL_Renderer *r) {
    switch (state) {
        case Menu:
            draw_menu(r);
            break;
        case Game:
            draw_ingame(r);
            break;
        case Pause:
            draw_pause(r);
            break;
    }
}

void cleanup_game() {
    cleanup_spritesheet();
    cleanup_ui();
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(startGameText);
    SDL_DestroyTexture(pausedGameText);
}
