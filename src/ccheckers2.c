#include <SDL.h>
#include <SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include "render.h"
#include "utils.h"
#include "game.h"
#include "input.h"
#include "pawn.h"
#include "board.h"
#include "bot.h"

static const int fps = 15;
static double interval = 0.0;

void loop(Game *state)
{
    Uint64 start = SDL_GetTicks64();

    input_exec(state);
    bot_exec(state);
    render_exec(state);
    board_exec(state);
    pawn_exec(state);

    Uint64 current = SDL_GetTicks64();

    Uint64 step = current - start;

    Uint32 delay = step > interval
        ? clamp_uint32(interval - step, 0, interval)
        : 0;

    state->step = step;

    SDL_Delay(delay);
}

int main(int argc, char* args[])
{
    printf("ccheckers2\n");

    interval = 1.0 / fps * 1000.0;

    game_init();
    Game *state = game_get_state();

    bot_init(state);
    pawn_init(state);
    board_init();
    render_init(state);

    // game_start();

    while (!state->is_quit)
    {
        loop(state);
    }

    game_quit();
    render_quit();
    bot_quit();
    pawn_quit();
    board_quit();

    SDL_Quit();

    return 0;
}