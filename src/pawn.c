#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "pawn.h"
#include "board.h"

static Game* game_state;

Pawn* pawn_get_all()
{
    return game_state->pawns;
}

void pawn_init(Game* state)
{
    game_state = state;

    int id = 0;
    for (int i = 0; i < 3; i++)
    {
        int start = i % 2 > 0 ? 1 : 0;
        for (int j = start; j < 8;)
        {
            game_state->pawns[id].id = id;
            game_state->pawns[id].colour = 0;
            game_state->pawns[id].x = j;
            game_state->pawns[id].y = i;
            game_state->pawns[id].radius = 0;
            game_state->pawns[id].direction = 1;
            game_state->pawns[id].is_king = false;
            game_state->pawns[id].is_hover = false;
            game_state->pawns[id].is_selected = false;
            game_state->pawns[id].is_active = true;

            id++;
            j = j + 2;
        }
    }

    for (int i = 5; i < 8; i++)
    {
        int start = i % 2 > 0 ? 1 : 0;
        for (int j = start; j < 8;)
        {
            game_state->pawns[id].id = id;
            game_state->pawns[id].colour = 1;
            game_state->pawns[id].x = j;
            game_state->pawns[id].y = i;
            game_state->pawns[id].radius = 0;
            game_state->pawns[id].direction = -1;
            game_state->pawns[id].is_king = false;
            game_state->pawns[id].is_hover = false;
            game_state->pawns[id].is_selected = false;
            game_state->pawns[id].is_active = true;

            id++;
            j = j + 2;
        }
    }
}

void pawn_deselect_all()
{
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        game_state->selected_pawn_id = -1;
        game_state->pawns[i].is_selected = false;
    }
}

void pawn_get_moves(Pawn* pawn, Grid* grid_1, Grid* grid_2, Grid* grid_3, Grid* grid_4)
{
    int grid_x = board_x_to_grid(pawn->grid_x);
    int grid_y = board_y_to_grid(pawn->grid_y);

    // printf("5 grid_x: %d, grid_y: %d\n", grid_x, grid_y);

    grid_1->x = grid_x + 1;
    grid_1->y = grid_y + (1 * pawn->direction);

    grid_2->x = grid_x - 1;
    grid_2->y = grid_y + (1 * pawn->direction);

    grid_3->x = grid_x - 2;
    grid_3->y = grid_y + (2 * pawn->direction);

    grid_4->x = grid_x + 2;
    grid_4->y = grid_y + (2 * pawn->direction);
}

int pawn_mouse_click(Pawn* pawn)
{
    if (pawn->is_hover)
    {
        if (pawn->is_selected)
        {
            pawn_deselect_all();
        }
        else
        {
            pawn_deselect_all();
            pawn->is_selected = true;
            game_state->selected_pawn_id = pawn->id;
            return pawn->id;
        }
    }
    return -1;
}

void pawn_mouse_hover(Pawn* pawn, int x, int y)
{
    double dist = abs(x - pawn->grid_x) + abs(y - pawn->grid_y);

    pawn->is_hover = dist <= pawn->radius;
}

bool pawn_mouse_hover_by_grid(Pawn* pawn, int grid_x, int grid_y)
{
    pawn->is_hover = pawn->x == grid_x && pawn->y == grid_y;
    return pawn->is_hover;
}

Pawn* pawn_get_by_id(int id)
{

    for (int i = 0; i < game_state->pawn_count; i++)
    {
        if (game_state->pawns[i].id == id)
        {
            return &(game_state->pawns[i]);
        }
    }

    return NULL;
}

bool pawn_is_at_location(int x, int y)
{
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        if (x == game_state->pawns[i].grid_x
            && y == game_state->pawns[i].grid_y)
        {
            return true;
        }
    }
    return false;
}

bool pawn_is_at_location_grid(int grid_x, int grid_y)
{
    // printf("\n-------------\n");

    // printf("4 grid_x: %d, grid_y: %d\n", grid_x, grid_y);

    for (int i = 0; i < game_state->pawn_count; i++)
    {
        // printf("3 grid_x: %d, grid_y: %d\n", game_state->pawns[i].x, game_state->pawns[i].y);

        if (grid_x == game_state->pawns[i].x
            && grid_y == game_state->pawns[i].y)
        {
            // printf("1 grid_x: %d, grid_y: %d\n", grid_x, grid_y);

            return true;
        }
    }
    return false;
}

void pawn_set_x(Pawn* pawn, int x, int snapped_center_x)
{
    // printf("x: %d %d\n", x, snapped_center_x);

    pawn->grid_x = snapped_center_x;
    pawn->x = x;
}

void pawn_set_y(Pawn* pawn, int y, int snapped_center_y)
{
    // printf("y: %d %d\n", y, snapped_center_y);

    pawn->grid_y = snapped_center_y;
    pawn->y = y;

}

void pawn_exec()
{

}

void pawn_quit()
{

}

