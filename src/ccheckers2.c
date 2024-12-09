#include <SDL.h>
#include <SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include "render.h"
#include "utils.h"
#include "game.h"
#include "input.h"
#include "pawn.h"

static const int fps = 30;
static double interval = 0.0;

void loop(Game *state)
{
    // printf(".");

    Uint64 start = SDL_GetTicks64();

    input_exec(state);
    render_exec(state);

    Uint64 elapsed = SDL_GetTicks64() - start;

    Uint32 delay = elapsed > interval
        ? clamp_uint32(interval - elapsed, 0, interval)
        : 0;

    SDL_Delay(delay);
}

int main(int argc, char* args[])
{
    printf("ccheckers2\n");

    interval = 1.0 / fps * 1000.0;

    game_init();
    Game *state = game_get_state();

    pawn_init(state);
    render_init(state);

    while (!state->is_quit)
    {
        loop(state);
    }

    game_quit();
    render_quit();
    pawn_quit();

    SDL_Quit();

    return 0;
}