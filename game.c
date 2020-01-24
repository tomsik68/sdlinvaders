#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600

#define NINVADERS 64

#include "spritesheet.h"

typedef struct game {
    SDL_Rect ship;

    unsigned last_move;
    unsigned last_inv_move;

    SDL_Rect inv;
    sprite_t invader_type[NINVADERS];
} game_t;

static game_t game;

void init_game(SDL_Renderer *r) {
    init_spritesheet(r);

    game.ship.x = WIDTH / 2 - spritew(SpaceshipSprite) / 2;
    game.ship.y = HEIGHT - spriteh(SpaceshipSprite);
    game.ship.w = spritew(SpaceshipSprite);
    game.ship.h = spriteh(SpaceshipSprite);

    game.inv.x = -spritew(InvaderSprite);
    game.inv.y = 0;

    game.inv.w = spritew(InvaderSprite);
    game.inv.h = spriteh(InvaderSprite);

    game.last_move = 0;
    game.last_inv_move = 0;

    for (size_t i = 0; i < NINVADERS; ++i) {
        game.invader_type[i] = InvaderSprite;
    }
}

void handle_event(SDL_Event *event) {
    // TODO
}

static void draw_invaders(SDL_Renderer *r) {
    sprite_t s = InvaderSprite;

    SDL_Rect pos = game.inv;
    for (int i = 0; i < NINVADERS; ++i) {
        if (game.invader_type[i] != EmptySprite) {
            s = game.invader_type[i];

            pos.x = game.inv.x + (spritew(s) + 4) * i;
            pos.y = game.inv.y + (pos.x / (WIDTH + 2 * spritew(s))) *
                                     (spriteh(s) + 4);
            pos.x %= WIDTH + 2 * spritew(s);
            pos.x -= spritew(s);

            if (SDL_GetTicks() % 1000 > 500) {
                ++s;
            }

            draw_sprite(r, s, &pos);
        }
    }
}

static void move_ship() {
    if (SDL_GetTicks() - game.last_move > 10) {
        const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

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

static void move_invaders() {
    if (SDL_GetTicks() - game.last_inv_move > 15) {
        game.inv.x += 1;
        game.last_inv_move = SDL_GetTicks();

        if (game.inv.x == WIDTH + spritew(InvaderSprite)) {
            game.inv.x = -spritew(InvaderSprite);
            game.inv.y += spriteh(InvaderSprite) + 3;
        }
    }
}

void draw_game(SDL_Renderer *r) {
    draw_sprite(r, SpaceshipSprite, &game.ship);
    draw_invaders(r);
    move_ship();
    move_invaders();
}

void cleanup_game() { cleanup_spritesheet(); }
