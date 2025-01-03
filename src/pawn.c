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

void pawn_get_moves(Pawn* pawn, Grid* grid_1, Grid* grid_2, Grid* grid_3, Grid* grid_4,
    Grid* grid_5, Grid* grid_6, Grid* grid_7, Grid* grid_8)
{
    int grid_x = board_x_to_grid(pawn->grid_x);
    int grid_y = board_y_to_grid(pawn->grid_y);

    // printf("5 grid_x: %d, grid_y: %d\n", grid_x, grid_y);

    grid_1->x = grid_x + 1;
    grid_1->y = grid_y + (1 * pawn->direction);

    grid_2->x = grid_x - 1;
    grid_2->y = grid_y + (1 * pawn->direction);

    grid_3->x = grid_x + 2;
    grid_3->y = grid_y + (2 * pawn->direction);

    grid_4->x = grid_x - 2;
    grid_4->y = grid_y + (2 * pawn->direction);

    if (pawn->is_king)
    {
        grid_5->x = grid_x + 1;
        grid_5->y = grid_y - (1 * pawn->direction);

        grid_6->x = grid_x - 1;
        grid_6->y = grid_y - (1 * pawn->direction);

        grid_7->x = grid_x + 2;
        grid_7->y = grid_y - (2 * pawn->direction);

        grid_8->x = grid_x - 2;
        grid_8->y = grid_y - (2 * pawn->direction);
    }
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

bool pawn_is_at_location_grid(Grid* grid)
{
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        if (grid->x == game_state->pawns[i].x
            && grid->y == game_state->pawns[i].y)
        {
            return true;
        }
    }
    return false;
}

void pawn_set_x(Pawn* pawn, int x, int snapped_center_x)
{
    pawn->grid_x = snapped_center_x;
    pawn->x = x;
}

void pawn_set_y(Pawn* pawn, int y, int snapped_center_y)
{
    pawn->grid_y = snapped_center_y;
    pawn->y = y;

    if ((pawn->direction > 0 && y == 7)
        || (pawn->direction < 0 && y == 0))
    {
        pawn->is_king = true;
    }
}

bool pawn_is_opposition(int colour, Grid* grid)
{
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        if (grid->x == game_state->pawns[i].x
            && grid->y == game_state->pawns[i].y
            && colour != game_state->pawns[i].colour)
        {
            return true;
        }
    }

    return false;
}

void pawn_capture_move(Pawn* pawn, Grid* grid)
{
    // printf("grid_x: %d\n", grid->x);
    // printf("grid_y: %d\n", grid->y);

    // printf("pawn_x: %d\n", pawn->x);
    // printf("pawn_y: %d\n", pawn->y);

    int capture_x = grid->x > pawn->x ? pawn->x + 1 : pawn->x - 1;
    int capture_y = grid->y > pawn->y ? pawn->y + 1 : pawn->y - 1;

    for (int i = 0; i < game_state->pawn_count; i++)
    {
        if (game_state->pawns[i].is_active
            && capture_x == game_state->pawns[i].x
            && capture_y == game_state->pawns[i].y)
        {
            // printf("pawn_id1: %d\n", pawn->id);
            // printf("pawn_id2: %d\n", game_state->pawns[i].id);
            // printf("capture_x: %d\n", capture_x);
            // printf("capture_y: %d\n", capture_y);

            game_state->pawns[i].is_active = false;
            game_state->pawns[i].x = -1;
            game_state->pawns[i].y = -1;
            game_state->pawns[i].grid_x = -1;
            game_state->pawns[i].grid_y = -1;

            return;
        }
    }
}

bool pawn_is_capture_available(int pawn_colour, Grid* grid1, Grid* grid2)
{
    return pawn_is_at_location_grid(grid1)
        && !pawn_is_at_location_grid(grid2)
        && pawn_is_opposition(pawn_colour, grid1);
}

bool pawn_is_valid_capture(int pawn_colour, Grid* selected_grid, Grid* grid1, Grid* grid2)
{
    return selected_grid->x == grid2->x && selected_grid->y == grid2->y
        && pawn_is_at_location_grid(grid1)
        && !pawn_is_at_location_grid(grid2)
        && pawn_is_opposition(pawn_colour, grid1);
}

bool pawn_is_valid_move(Grid* selected_grid, Grid* grid1)
{
    return selected_grid->x == grid1->x && selected_grid->y == grid1->y;
}

void pawn_exec()
{

}

void pawn_quit()
{

}

