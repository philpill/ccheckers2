#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"
#include "pawn.h"

static Game *game_state;

Pawn* pawn_get_all()
{
    return game_state->pawns;
}

void pawn_init(Game *state)
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

            id++;
            j = j + 2;
        }
    }
}

void pawn_mouse_event(int x, int y, Uint32 mouse_state)
{
    // printf("pawn_mouse_event x: %d, y: %d, mouse: %d\n", x, y, mouse_state);

    for (int i = 0; i < 24; i++)
    {
        if (game_state->pawns[i].is_active)
        {
            double dist = abs(x - game_state->pawns[i].grid_x) + abs(y - game_state->pawns[i].grid_y);

            if (dist <= game_state->pawns[i].radius)
            {
                game_state->pawns[i].is_hover = true;
                if (mouse_state == SDL_BUTTON_LEFT)
                {
                    game_state->pawns[i].is_selected = true;
                }
            }
            else
            {
                game_state->pawns[i].is_hover = false;
                if (mouse_state == SDL_BUTTON_LEFT)
                {
                    game_state->pawns[i].is_selected = false;
                }
            }
        }
    }
}

void pawn_quit()
{

}

