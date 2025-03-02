#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"
#include "pawn.h"
#include "board.h"
#include "utils.h"

static Game* game_state;

Pawn* pawn_get_all()
{
    return game_state->pawns;
}

void pawn_initialise_pawns()
{
    game_state->pawn_count = 24;

    int id = 0;
    for (int i = 0; i < 3; i++)
    {
        int start = i % 2 > 0 ? 1 : 0;
        for (int j = start; j < 8;)
        {
            game_state->pawns[id].id = id;
            game_state->pawns[id].colour = 0;
            game_state->pawns[id].dest_x = -1;
            game_state->pawns[id].dest_y = -1;
            game_state->pawns[id].grid_x = j;
            game_state->pawns[id].grid_y = i;
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
            game_state->pawns[id].dest_x = -1;
            game_state->pawns[id].dest_y = -1;
            game_state->pawns[id].grid_x = j;
            game_state->pawns[id].grid_y = i;
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

void pawn_initialise_pawns_test()
{
    game_state->pawn_count = 2;

    game_state->pawns[0].id = 0;
    game_state->pawns[0].colour = 0;
    game_state->pawns[0].dest_x = -1;
    game_state->pawns[0].dest_y = -1;
    game_state->pawns[0].grid_x = 0;
    game_state->pawns[0].grid_y = 0;
    game_state->pawns[0].radius = 0;
    game_state->pawns[0].direction = 1;
    game_state->pawns[0].is_king = false;
    game_state->pawns[0].is_hover = false;
    game_state->pawns[0].is_selected = false;
    game_state->pawns[0].is_active = true;

    game_state->pawns[1].id = 1;
    game_state->pawns[1].colour = 1;
    game_state->pawns[1].dest_x = -1;
    game_state->pawns[1].dest_y = -1;
    game_state->pawns[1].grid_x = 1;
    game_state->pawns[1].grid_y = 1;
    game_state->pawns[1].radius = 0;
    game_state->pawns[1].direction = -1;
    game_state->pawns[1].is_king = false;
    game_state->pawns[1].is_hover = false;
    game_state->pawns[1].is_selected = false;
    game_state->pawns[1].is_active = true;
}

void pawn_init(Game* state)
{
    game_state = state;
}

void pawn_deselect_all()
{
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        game_state->selected_pawn_id = -1;
        game_state->pawns[i].is_selected = false;
    }
}

void pawn_get_moves(Pawn* pawn, Grid* grids)
{
    int grid_x = board_x_to_grid(pawn->x);
    int grid_y = board_y_to_grid(pawn->y);

    // printf("5 grid_x: %d, grid_y: %d\n", grid_x, grid_y);

    (grids[0]).x = grid_x + 1;
    (grids[0]).y = grid_y + (1 * pawn->direction);

    (grids[1]).x = grid_x - 1;
    (grids[1]).y = grid_y + (1 * pawn->direction);

    (grids[2]).x = grid_x + 2;
    (grids[2]).y = grid_y + (2 * pawn->direction);

    (grids[3]).x = grid_x - 2;
    (grids[3]).y = grid_y + (2 * pawn->direction);

    if (pawn->is_king)
    {
        (grids[4]).x = grid_x + 1;
        (grids[4]).y = grid_y - (1 * pawn->direction);

        (grids[5]).x = grid_x - 1;
        (grids[5]).y = grid_y - (1 * pawn->direction);

        (grids[6]).x = grid_x + 2;
        (grids[6]).y = grid_y - (2 * pawn->direction);

        (grids[7]).x = grid_x - 2;
        (grids[7]).y = grid_y - (2 * pawn->direction);
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
    double dist = abs(x - pawn->x) + abs(y - pawn->y);

    pawn->is_hover = dist <= pawn->radius;
}

bool pawn_mouse_hover_by_grid(Pawn* pawn, int grid_x, int grid_y)
{
    pawn->is_hover = pawn->grid_x == grid_x && pawn->grid_y == grid_y;
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
        if (grid->x == game_state->pawns[i].grid_x
            && grid->y == game_state->pawns[i].grid_y)
        {
            return true;
        }
    }
    return false;
}

void pawn_set_x(Pawn* pawn, int grid_x, int x)
{
    // pawn->x = x;
    // pawn->grid_x = grid_x;
}

void pawn_set_y(Pawn* pawn, int grid_y, int y)
{
    // pawn->y = y;
    // pawn->grid_y = grid_y;

    // if ((pawn->direction > 0 && grid_y == 7)
    //     || (pawn->direction < 0 && grid_y == 0))
    // {
    //     pawn->is_king = true;
    // }
}

void pawn_set_dest_x(Pawn* pawn, int dest_x)
{
    pawn->dest_x = dest_x;
}

void pawn_set_dest_y(Pawn* pawn, int dest_y)
{
    pawn->dest_y = dest_y;
}

void pawn_resolve_position(Pawn* pawn)
{
    if (pawn->dest_x > -1)
    {
        if (pawn->x == pawn->dest_x)
        {
            pawn->dest_x = -1;
            pawn->grid_x = board_x_to_grid(pawn->x);
        }
        else
        {
            float distance = game_state->animation_speed * game_state->step;

            pawn->x = pawn->x > pawn->dest_x
                ? pawn->x - distance
                : pawn->x + distance;

            pawn->x = round_float(pawn->x);
        }
    }

    if (pawn->dest_y > -1)
    {
        if (pawn->y == pawn->dest_y && pawn->dest_y > -1)
        {
            pawn->dest_y = -1;

            int grid_y = board_y_to_grid(pawn->y);
            pawn->grid_y = grid_y;

            if ((pawn->direction > 0 && grid_y == 7)
                || (pawn->direction < 0 && grid_y == 0))
            {
                pawn->is_king = true;
            }
        }
        else
        {
            float distance = game_state->animation_speed * game_state->step;

            pawn->y = pawn->y > pawn->dest_y
                ? pawn->y - distance
                : pawn->y + distance;

            pawn->y = round_float(pawn->y);
        }
    }
}

bool pawn_is_opposition(int colour, Grid* grid)
{
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        if (grid->x == game_state->pawns[i].grid_x
            && grid->y == game_state->pawns[i].grid_y
            && colour != game_state->pawns[i].colour)
        {
            return true;
        }
    }

    return false;
}

void pawn_capture_move(Pawn* pawn, Grid* grid)
{
    int capture_x = grid->x > pawn->grid_x ? pawn->grid_x + 1 : pawn->grid_x - 1;
    int capture_y = grid->y > pawn->grid_y ? pawn->grid_y + 1 : pawn->grid_y - 1;

    for (int i = 0; i < game_state->pawn_count; i++)
    {
        if (game_state->pawns[i].is_active
            && capture_x == game_state->pawns[i].grid_x
            && capture_y == game_state->pawns[i].grid_y)
        {
            pawn_capture(&(game_state->pawns[i]));
            return;
        }
    }
}

void pawn_capture(Pawn* pawn)
{
    pawn->is_active = false;
    pawn->grid_x = -1;
    pawn->grid_y = -1;
    pawn->x = -1;
    pawn->y = -1;
    pawn->dest_x = -1;
    pawn->dest_y = -1;
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
    bool is_animating = false;
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        pawn_resolve_position(&(game_state->pawns[i]));
        is_animating = is_animating
            &&
            (game_state->pawns[i].x != game_state->pawns[i].dest_x
                || game_state->pawns[i].y != game_state->pawns[i].dest_y);
    }
    game_state->is_animating = is_animating;
}

void pawn_quit()
{

}

