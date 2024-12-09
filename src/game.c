#include <stdlib.h>
#include "game.h"
#include "pawn.h"

static Game game_state;

void game_init()
{
    game_state.id = 1;
    game_state.is_quit = false;
    game_state.pawns = malloc(24 * sizeof(Pawn));
}

Game* game_get_state() 
{
    return &game_state;
}

void game_quit()
{
    free(game_state.pawns);
}

