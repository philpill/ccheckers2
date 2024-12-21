#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "pawn.h"

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
            game_state->pawns[id].is_king = false;
            game_state->pawns[id].is_hover = false;
            game_state->pawns[id].is_selected = false;
            game_state->pawns[id].is_active = true;
            game_state->pawns[id].has_moved = false;

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

void pawn_mouse_click(Pawn* pawn)
{
    if (pawn->is_hover)
    {
        pawn_deselect_all();
        pawn->is_selected = true;
        game_state->selected_pawn_id = pawn->id;
    }
}

void pawn_mouse_hover(Pawn* pawn, int x, int y)
{
    double dist = abs(x - pawn->grid_x) + abs(y - pawn->grid_y);

    pawn->is_hover = dist <= pawn->radius;
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

void pawn_exec()
{
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        if (game_state->pawns[i].has_moved && game_state->pawns[i].is_selected) 
        {
            game_state->pawns[i].is_selected = false;
            game_state->selected_pawn_id = -1;

        }
    }
}

void pawn_quit()
{

}

