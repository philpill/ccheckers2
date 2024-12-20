#include <stdlib.h>
#include "board.h"
#include "game.h"

static Game* game_state;

void board_init(Game* state)
{
    game_state = state;

    int id = 0;

    for (int i = 0; i < 4; i++)
    {
        game_state->movement_tiles[i].id = id++;
        game_state->movement_tiles[i].x = 0;
        game_state->movement_tiles[i].y = 0;
        game_state->movement_tiles[i].size = game_state->grid_size;
        game_state->movement_tiles[i].colour = 0;
        game_state->movement_tiles[i].is_enabled = false;
    }

    game_state->active_tile->id = id++;
    game_state->active_tile->x = 0;
    game_state->active_tile->y = 0;
    game_state->active_tile->size = game_state->grid_size;
    game_state->active_tile->colour = 0;
    game_state->active_tile->is_enabled = false;

    game_state->cursor_tile->id = id++;
    game_state->cursor_tile->x = 0;
    game_state->cursor_tile->y = 0;
    game_state->cursor_tile->size = game_state->grid_size;
    game_state->cursor_tile->colour = 0;
    game_state->cursor_tile->is_enabled = false;

}

void board_exec(Game* state)
{

}

void board_mouse_hover(int x, int y)
{
    int tile_x = (x - game_state->board_offset_x) / game_state->grid_size;
    int tile_y = (y - game_state->board_offset_y)/ game_state->grid_size;
 
    game_state->cursor_tile->x = tile_x
        * game_state->grid_size
        + game_state->board_offset_x;
    game_state->cursor_tile->y = tile_y 
        * game_state->grid_size
        + game_state->board_offset_y;
}

void board_mouse_click(int x, int y)
{

}

void board_quit()
{

}