#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "pawn.h"

static Game* game_state;

static MaxRating ratings[64];

static int max_rating;

int bot_get_grid_rating(int grid_rating_index, int colour, Grid current_grid, Grid dest_grid)
{
    int rating = 100;

    game_state->grid_ratings[grid_rating_index] = rating;

    return rating;
}

void bot_clear_grid_ratings()
{
    for (int i = 0;i < 8;i++)
    {
        game_state->grid_ratings[i] = -1;
    }
}

void populate_ratings(Pawn* pawn, int count)
{
    if (pawn->colour == game_state->current_colour)
    {
        Grid grids[8];

        for (int i = 0; i < 8; i++)
        {
            grids[i].x = -1;
            grids[i].y = -1;
        }

        pawn_get_moves(pawn, grids);

        Grid current_grid = { pawn->x, pawn->y };
        int id = pawn->id;
        int rating_id = (id * 8) + count;

        for (int i = 0; i < 8; i++)
        {
            ratings[rating_id].rating_id = rating_id;
            ratings[rating_id].pawn_id = id;
            ratings[rating_id].grid_index = 0;
            ratings[rating_id].rating = bot_get_grid_rating(i, game_state->current_colour, current_grid, grids[i]);
        }
    }
}

void bot_exec()
{
    if ((game_state->is_player1_bot && game_state->current_colour == -1)
        || (game_state->is_player2_bot && game_state->current_colour == 1))
    {
        int pawn_count = game_state->pawn_count;

        for (int i = 0; i < pawn_count; i++)
        {
            populate_ratings(&(game_state->pawns[i]), i);
        }

        // get all pawns
        // for each pawn:
            // get all moves
            // for each move
                // get grid rating
        // select pawn + grid rating
        // move pawn

        bot_clear_grid_ratings();
    }
}

void bot_init(Game* state)
{
    game_state = state;
}

void bot_quit()
{
}