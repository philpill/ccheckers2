#include <stdlib.h>
#include <sdl.h>
#include "game.h"
#include "pawn.h"
#include "board.h"

static Game game_state;

void game_init()
{
    game_state.id = 1;
    game_state.grid_size = 32;
    game_state.is_quit = false;
    game_state.pawns = malloc(24 * sizeof(Pawn));

    game_state.movement_tiles = malloc(4 * sizeof(Tile));
    game_state.active_tile = malloc(sizeof(Tile));
    game_state.cursor_tile = malloc(sizeof(Tile));
}

Game* game_get_state()
{
    return &game_state;
}

void game_quit()
{
    free(game_state.pawns);
    free(game_state.movement_tiles);
    free(game_state.active_tile);
    free(game_state.cursor_tile);
}

void game_mouse_event(int x, int y, Uint32 mouse_state)
{
    bool is_click = mouse_state == SDL_BUTTON_LEFT;

    board_mouse_hover(x, y);

    if (is_click)
    {
        board_mouse_click(x, y);
    }

    for (int i = 0; i < 24; i++)
    {
        if (game_state.pawns[i].is_active)
        {
            pawn_mouse_hover(&game_state.pawns[i], x, y);

            if (is_click)
            {
                game_state.pawns[i].is_selected = false;
                pawn_mouse_click(&game_state.pawns[i]);
            }
        }
    }
}