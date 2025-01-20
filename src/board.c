#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "game.h"

static Game* game_state;

int board_grid_to_x(int grid)
{
    int x = (grid * game_state->grid_size) + game_state->board_offset_x;

    return x;
}

int board_grid_to_y(int grid)
{
    int y = (grid * game_state->grid_size) + game_state->board_offset_y;

    return y;
}

int board_x_to_grid(int x)
{
    int grid = (x - game_state->board_offset_x) / game_state->grid_size;

    return grid;
}

int board_y_to_grid(int y)
{
    int grid = (y - game_state->board_offset_y) / game_state->grid_size;

    return grid;
}

int board_get_snapped_x(int x)
{
    int tile_pos = (x - game_state->board_offset_x) / game_state->grid_size;
    int snapped_pos = tile_pos
        * game_state->grid_size
        + game_state->board_offset_x;
    return snapped_pos;
}

int board_get_snapped_y(int y)
{
    int tile_pos = (y - game_state->board_offset_y) / game_state->grid_size;
    int snapped_pos = tile_pos
        * game_state->grid_size
        + game_state->board_offset_y;
    return snapped_pos;
}

int board_get_snapped_center_x(int x)
{
    int snapped_pos = board_get_snapped_x(x) + game_state->grid_size / 2;
    return snapped_pos;
}

int board_get_snapped_center_y(int y)
{
    int snapped_pos = board_get_snapped_y(y) + game_state->grid_size / 2;
    return snapped_pos;
}

void board_init(Game* state)
{
    game_state = state;

    game_state->grid_size = 32;

    game_state->board_bound_x = (game_state->grid_size * 8) + game_state->board_offset_x;
    game_state->board_bound_y = (game_state->grid_size * 8) + game_state->board_offset_y;

    int id = 0;

    for (int i = 0; i < 8; i++)
    {
        game_state->movement_tiles[i].id = id++;
        game_state->movement_tiles[i].x = board_get_snapped_x(0);
        game_state->movement_tiles[i].y = board_get_snapped_x(0);
        game_state->movement_tiles[i].size = game_state->grid_size;
        game_state->movement_tiles[i].colour = 3;
        game_state->movement_tiles[i].is_enabled = false;
    }

    game_state->active_tile->id = id++;
    game_state->active_tile->x = board_get_snapped_x(0);
    game_state->active_tile->y = board_get_snapped_x(0);
    game_state->active_tile->size = game_state->grid_size;
    game_state->active_tile->colour = 5;
    game_state->active_tile->is_enabled = false;

    game_state->cursor_tile->id = id++;
    game_state->cursor_tile->x = board_get_snapped_x(0);
    game_state->cursor_tile->y = board_get_snapped_y(0);
    game_state->cursor_tile->size = game_state->grid_size;
    game_state->cursor_tile->colour = 4;
    game_state->cursor_tile->is_enabled = false;
}

void board_exec(Game* state)
{

}

void board_mouse_hover(int snapped_x, int snapped_y)
{
    game_state->cursor_tile->x = snapped_x;
    game_state->cursor_tile->y = snapped_y;
}

void board_mouse_click(int snapped_x, int snapped_y)
{
    game_state->active_tile->is_enabled = true;
    game_state->active_tile->x = snapped_x;
    game_state->active_tile->y = snapped_y;
}

void board_set_movement_tile_grid(int tile_index, Grid grid)
{
    int x = board_grid_to_x(grid.x);
    int y = board_grid_to_y(grid.y);

    game_state->movement_tiles[tile_index].x = x;
    game_state->movement_tiles[tile_index].y = y;
    game_state->movement_tiles[tile_index].is_enabled
        = grid.x > -1
        && grid.x < 8
        && grid.y > -1
        && grid.y < 8;
}

void board_clear_tile_grids()
{
    int snapped_x = board_get_snapped_x(-1);
    int snapped_y = board_get_snapped_y(-1);

    for (int i = 0;i < 8;i++)
    {
        game_state->movement_tiles[i].x = snapped_x;
        game_state->movement_tiles[i].y = snapped_y;
        game_state->movement_tiles[i].is_enabled = false;
    }

    game_state->active_tile->is_enabled = false;
    game_state->active_tile->x = snapped_x;
    game_state->active_tile->y = snapped_y;
}

void board_quit()
{

}