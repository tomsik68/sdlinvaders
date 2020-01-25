#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

#define NINVADERS 64
#define NMAXSHOT 8
#define NINVADER_TYPES 11

#include "spritesheet.h"

typedef struct game {
    SDL_Rect ship;
    SDL_Rect shots[NMAXSHOT];

    unsigned last_move;
    unsigned last_inv_move;
    unsigned last_shot;

    SDL_Rect inv;
    sprite_t invader_type[NINVADERS];
} game_t;

static game_t game;

static void place_ship() {
    game.ship.x = WIDTH / 2 - spritew(SpaceshipSprite) / 2;
    game.ship.y = HEIGHT - spriteh(SpaceshipSprite);
    game.ship.w = spritew(SpaceshipSprite);
    game.ship.h = spriteh(SpaceshipSprite);
}

static void init_invaders() {
    game.inv.x = -spritew(InvaderSprite);
    game.inv.y = 0;
    game.inv.w = spritew(InvaderSprite);
    game.inv.h = spriteh(InvaderSprite);

    for (size_t i = 0; i < NINVADERS; ++i) {
        game.invader_type[i] = InvaderSprite + 2 * (rand() % NINVADER_TYPES);
    }
}

static void init_shots() {
    for (size_t i = 0; i < NMAXSHOT; ++i) {
        game.shots[i].x = -999;
        game.shots[i].y = -999;
        game.shots[i].w = 4;
        game.shots[i].h = 8;
    }
}

void init_game(SDL_Renderer *r) {
    srand(time(NULL));
    init_spritesheet(r);
    place_ship();
    init_invaders();
    init_shots();

    game.last_move = 0;
    game.last_inv_move = 0;
    game.last_shot = 0;
}

void handle_event(SDL_Event *event) {
    // TODO
}

static void compute_invader_pos(size_t i, SDL_Rect *pos) {
    sprite_t s = game.invader_type[i];
    pos->x = game.inv.x + (spritew(InvaderSprite) + 4) * i;
    pos->y =
        game.inv.y + (pos->x / (WIDTH + 2 * spritew(InvaderSprite))) * (spriteh(InvaderSprite) + 4);
    pos->x %= WIDTH + 2 * spritew(InvaderSprite);
    pos->x -= spritew(InvaderSprite);
    pos->w = spritew(s);
    pos->h = spriteh(s);
}

static void draw_invaders(SDL_Renderer *r) {
    sprite_t s = InvaderSprite;

    SDL_Rect pos = game.inv;
    for (size_t i = 0; i < NINVADERS; ++i) {
        if (game.invader_type[i] != EmptySprite) {
            s = game.invader_type[i];
            compute_invader_pos(i, &pos);

            if (SDL_GetTicks() % 1000 > 500) {
                ++s;
            }

            draw_sprite(r, s, &pos);
        }
    }
}

static void shoot() {
    if (SDL_GetTicks() - game.last_shot > 200) {
        for (size_t i = 0; i < NMAXSHOT; ++i) {
            if (game.shots[i].x == -999) {
                game.shots[i].x = game.ship.x + spritew(SpaceshipSprite) / 2;
                game.shots[i].y = game.ship.y - 4;
                break;
            }
        }

        game.last_shot = SDL_GetTicks();
    }
}

static void move_ship() {
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    if (keyboard[SDL_SCANCODE_SPACE]) {
        shoot();
    }

    if (SDL_GetTicks() - game.last_move > 10) {
        if (keyboard[SDL_SCANCODE_RIGHT]) {
            game.ship.x += 4;
        }
        if (keyboard[SDL_SCANCODE_LEFT]) {
            game.ship.x -= 4;
        }
        if (keyboard[SDL_SCANCODE_DOWN]) {
            game.ship.y += 4;
        }
        if (keyboard[SDL_SCANCODE_UP]) {
            game.ship.y -= 4;
        }

        game.last_move = SDL_GetTicks();
    }
}

static void move_shots() {
    SDL_Rect dummy;
    SDL_Rect invader;

    for (size_t i = 0; i < NMAXSHOT; ++i) {
        game.shots[i].y -= 2;

        if (game.shots[i].y < -10) {
            game.shots[i].x = -999;
            game.shots[i].y = -999;
        }

        if (game.shots[i].x != -999) {
            for (size_t j = 0; j < NINVADERS; ++j) {
                compute_invader_pos(j, &invader);
                if (SDL_IntersectRect(&game.shots[i], &invader, &dummy) ==
                    SDL_TRUE) {
                    game.invader_type[j] = EmptySprite;
                    game.shots[i].x = -999;
                    break;
                }
            }
        }
    }
}

static void move_invaders() {
    if (SDL_GetTicks() - game.last_inv_move > 15) {
        game.inv.x += 1;
        game.last_inv_move = SDL_GetTicks();

        if (game.inv.x == WIDTH + spritew(InvaderSprite)) {
            game.inv.x = -spritew(InvaderSprite);
            game.inv.y += spriteh(InvaderSprite) + 3;
        }
        move_shots();
    }
}

static void draw_shots(SDL_Renderer *r) {
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    for (size_t i = 0; i < NMAXSHOT; ++i) {
        SDL_RenderFillRect(r, &game.shots[i]);
    }

    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
}
void draw_game(SDL_Renderer *r) {
    draw_sprite(r, SpaceshipSprite, &game.ship);
    draw_invaders(r);
    draw_shots(r);

    move_ship();
    move_invaders();
}

void cleanup_game() { cleanup_spritesheet(); }
