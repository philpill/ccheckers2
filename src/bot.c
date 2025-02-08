#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "pawn.h"
#include "board.h"

static Game* game_state;

static MaxRating ratings[200];

static Grid grids[8];

static int rating_indexes[96];

static int max_rating = -1;

static time_t seconds_1 = -1;

// negative rating is illegal move
int bot_get_grid_rating(int pawn_id, int colour, Grid current_grid, Grid dest_grid)
{
    int rating = rand() % 10;

    bool is_valid_move = game_is_valid_move(pawn_id, dest_grid.x, dest_grid.y);
    bool is_valid_capture = game_is_valid_capture(pawn_id, dest_grid.x, dest_grid.y);

    if (is_valid_move && is_valid_capture)
    {
        rating = rating + 5;
    }

    if (!is_valid_move)
    {
        rating = -1;
    }

    return rating;
}

void bot_clear_grids()
{
    for (int i = 0; i < 8; i++)
    {
        grids[i].x = -1;
        grids[i].y = -1;
    }
}

void bot_clear_grid_ratings()
{
    for (int i = 0;i < 200; i++)
    {
        ratings[i].rating = -1;
    }

    for (int i = 0; i < 96; i++)
    {
        rating_indexes[i] = -1;
    }

    max_rating = -1;
}

void populate_ratings(Pawn* pawn, int count)
{
    if (pawn->colour == game_state->current_colour
        && pawn->is_active)
    {
        bot_clear_grids();

        pawn_get_moves(pawn, grids);

        Grid current_grid = { pawn->grid_x, pawn->grid_y };
        int id = pawn->id;
        for (int i = 0; i < 8; i++)
        {
            int rating_id = (id * 8) + i;
            ratings[rating_id].rating_id = rating_id;
            ratings[rating_id].pawn_id = id;
            ratings[rating_id].grid_index = i;
            ratings[rating_id].rating = bot_get_grid_rating(id, game_state->current_colour, current_grid, grids[i]);
        }
    }
}

void bot_end_turn()
{
    seconds_1 = -1;
    bot_clear_grids();
    bot_clear_grid_ratings();
    pawn_deselect_all();

    // switch active player
    game_state->current_colour = game_state->current_colour == 0 ? 1 : 0;
}

void bot_exec()
{
    int now = time(NULL);

    if (seconds_1 < 0) {
        seconds_1 = now;
    }

    if ((now - seconds_1) < 3)
    {
        return;
    }
    else
    {
        seconds_1 = now;
    }

    if ((game_state->is_player1_bot && game_state->current_colour == -1)
        || (game_state->is_player2_bot && game_state->current_colour == 1))
    {
        int pawn_count = game_state->pawn_count;
        for (int i = 0; i < pawn_count; i++)
        {
            populate_ratings(&(game_state->pawns[i]), i);
        }

        for (int i = 0; i < 200; i++)
        {
            // negative rating is illegal move
            if (ratings[i].rating > -1 && ratings[i].rating > max_rating) {
                max_rating = ratings[i].rating;
            }
        }

        int rating_count = 0;

        for (int i = 0; i < 200; i++)
        {
            if (ratings[i].rating == max_rating && max_rating != -1)
            {
                rating_indexes[rating_count++] = ratings[i].rating_id;
            }
        }

        if (rating_count == 0 || max_rating == -1)
        {
            // no valid move - current player loses
            game_state->stage = 2;
            printf("no valid move - current player loses\n");
        }
        else
        {
            int random_index = rand() % rating_count;

            int rating_index = rating_indexes[random_index];

            int mod = rating_index % 8;

            int id = (rating_index - mod) / 8;

            int grid_index = ratings[rating_index].grid_index;

            Pawn* pawn = pawn_get_by_id(id);

            pawn->is_selected = true;
            game_state->selected_pawn_id = pawn->id;

            bot_clear_grids();
            pawn_get_moves(pawn, grids);

            int grid_x = grids[grid_index].x;
            int grid_y = grids[grid_index].y;

            int x = board_grid_to_x(grid_x);
            int y = board_grid_to_y(grid_y);

            int snapped_center_x = board_get_snapped_center_x(x);
            int snapped_center_y = board_get_snapped_center_y(y);

            if (game_is_valid_capture(pawn->id, grid_x, grid_y))
            {
                pawn_capture_move(pawn, &(grids[grid_index]));
            }

            pawn_set_x(pawn, grid_x, snapped_center_x);
            pawn_set_y(pawn, grid_y, snapped_center_y);
        }

        // printf("bot pawn: %d, x: %d y: %d\n", pawn->id, grid_x, grid_y);

        bot_end_turn();
    }
}

void bot_init(Game* state)
{
    game_state = state;
    srand(time(NULL));
}

void bot_quit()
{
}