#include "game.h"

static Game game_state;

void game_init()
{
    game_state.id = 1;
    game_state.is_quit = false;
}

Game* game_get_state() 
{
    return &game_state;
}

void game_quit()
{

}

