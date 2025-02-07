#include <stdlib.h>
#include <sdl.h>
#include <stdio.h>
#include "game.h"
#include "pawn.h"
#include "board.h"
#include "render.h"
#include "bot.h"

static Game game_state;

static int options_count = 8;

void game_init()
{
    game_state.id = 1;
    game_state.board_offset_x = 10;
    game_state.board_offset_y = 10;
    game_state.current_colour = 0;
    game_state.selected_pawn_id = -1;
    game_state.pawn_count = 24;
    game_state.stage = 0;
    game_state.is_player1_bot = false;
    game_state.is_player2_bot = true;
    game_state.is_quit = false;
    game_state.pawns = malloc(24 * sizeof(Pawn));

    game_state.movement_tiles = malloc(8 * sizeof(Tile));
    game_state.active_tile = malloc(sizeof(Tile));
    game_state.cursor_tile = malloc(sizeof(Tile));

    game_state.options = malloc(options_count * sizeof(Option));
    for (int i = 0; i < options_count; i++)
    {
        game_state.options[i].id = i;
        game_state.options[i].group = 0;
        game_state.options[i].height = 0;
        game_state.options[i].width = 0;
        game_state.options[i].x1 = 0;
        game_state.options[i].x2 = 0;
        game_state.options[i].y1 = 0;
        game_state.options[i].y2 = 0;
        game_state.options[i].text = malloc(80 * sizeof(char));
    }
    game_state.selected_option_index = -1;

    game_state.options[0].text = "1P vs CPU";
    game_state.options[0].command = onevcpu;
    game_state.options[0].group = 1;
    game_state.options[1].text = "1P vs 2P";
    game_state.options[1].command = onevtwo;
    game_state.options[1].group = 1;
    game_state.options[2].text = "Exit";
    game_state.options[2].command = quit;
    game_state.options[2].group = 1;

    game_state.options[3].text = "Restart";
    game_state.options[3].command = restart;
    game_state.options[3].group = 2;
    game_state.options[4].text = "Exit";
    game_state.options[4].command = quit;
    game_state.options[4].group = 2;

    game_state.options[5].text = "Resume";
    game_state.options[5].command = resume;
    game_state.options[5].group = 3;
    game_state.options[6].text = "Restart";
    game_state.options[6].command = restart;
    game_state.options[6].group = 3;
    game_state.options[7].text = "Quit";
    game_state.options[7].command = quit;
    game_state.options[7].group = 3;
}

Game* game_get_state()
{
    return &game_state;
}

void game_start()
{
    game_state.stage = 1;
    game_state.cursor_tile->is_enabled = true;
}

void game_quit()
{
    free(game_state.pawns);
    free(game_state.movement_tiles);
    free(game_state.active_tile);
    free(game_state.cursor_tile);

    for (int i = 0; i < options_count; i++)
    {
        free(game_state.options[i].text);
    }

    free(game_state.options);
}

bool game_is_current_colour_bot(int current_colour)
{
    return (current_colour == -1 && game_state.is_player1_bot)
        || (current_colour == 1 && game_state.is_player2_bot);
}

void game_ui_hover(int x, int y)
{
    for (int i = 0; i < options_count; i++)
    {
        bool is_hover = false;

        if (game_state.stage == 0 && game_state.options[i].group == 1)
        {
            if (game_state.options[i].x1 < x
                && game_state.options[i].x2 > x
                && game_state.options[i].y1 < y
                && game_state.options[i].y2 > y)
            {
                is_hover = true;
            }
        }
        else if (game_state.stage == 2 && game_state.options[i].group == 2)
        {
            if (game_state.options[i].x1 < x
                && game_state.options[i].x2 > x
                && game_state.options[i].y1 < y
                && game_state.options[i].y2 > y)
            {
                is_hover = true;
            }
        }
        else if (game_state.stage == 3 && game_state.options[i].group == 3)
        {
            if (game_state.options[i].x1 < x
                && game_state.options[i].x2 > x
                && game_state.options[i].y1 < y
                && game_state.options[i].y2 > y)
            {
                is_hover = true;
            }
        }
        game_state.options[i].is_hover = is_hover;
        game_state.options[i].colour_index = is_hover ? 1 : 0;
    }
}

void game_mouse_hover(int x, int y)
{
    int grid_x = board_x_to_grid(x);
    int grid_y = board_x_to_grid(y);

    int snapped_x = board_get_snapped_x(x);
    int snapped_y = board_get_snapped_y(y);

    board_mouse_hover(snapped_x, snapped_y);

    for (int i = 0; i < game_state.pawn_count; i++)
    {
        if (game_state.pawns[i].is_active
            && game_state.pawns[i].colour == game_state.current_colour)
        {
            pawn_mouse_hover_by_grid(&game_state.pawns[i], grid_x, grid_y);
        }
    }
}

bool game_is_valid_move(int pawn_id, int grid_x, int grid_y)
{
    Grid selected_grid = { grid_x, grid_y };

    bool is_valid = false;

    Grid grids[8];

    Pawn* pawn = pawn_get_by_id(pawn_id);

    pawn_get_moves(pawn, grids);

    bool is_unoccupied = !pawn_is_at_location_grid(&selected_grid);

    bool is_in_bounds = grid_x > -1 && grid_x < 8 && grid_y > -1 && grid_y < 8;

    bool is_valid_move = pawn_is_valid_move(&selected_grid, &grids[0])
        || pawn_is_valid_move(&selected_grid, &grids[1])
        || (pawn->is_king && pawn_is_valid_move(&selected_grid, &grids[4]))
        || (pawn->is_king && pawn_is_valid_move(&selected_grid, &grids[5]));

    bool is_valid_capture
        = pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[0], &grids[2])
        || pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[1], &grids[3])
        || (pawn->is_king && pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[4], &grids[6]))
        || (pawn->is_king && pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[5], &grids[7]));

    is_valid = pawn != NULL
        && is_in_bounds
        && is_unoccupied
        && (is_valid_move || is_valid_capture);

    return is_valid;
}

bool game_is_valid_capture(int pawn_id, int grid_x, int grid_y)
{
    Grid selected_grid = { grid_x, grid_y };

    bool is_valid = false;

    Grid grids[8];

    Pawn* pawn = pawn_get_by_id(pawn_id);

    pawn_get_moves(pawn, grids);

    bool is_valid_a = pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[0], &grids[2]);
    bool is_valid_b = pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[1], &grids[3]);
    bool is_valid_c = (pawn->is_king && pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[4], &grids[6]));
    bool is_valid_d = (pawn->is_king && pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[5], &grids[7]));

    bool is_valid_capture
        = is_valid_a
        || is_valid_b
        || is_valid_c
        || is_valid_d;

    is_valid = pawn != NULL
        && is_valid_capture;

    return is_valid;
}

void game_ui_click(int x, int y)
{
    for (int i = 0; i < options_count; i++)
    {
        if (game_state.options[i].is_hover)
        {
            if (game_state.options[i].command == onevcpu)
            {
                pawn_initialise_pawns();
                render_init_pieces();

                game_state.stage = 1;
                game_state.is_player1_bot = false;
                game_state.is_player2_bot = true;
            }
            else if (game_state.options[i].command == onevtwo)
            {
                pawn_initialise_pawns();
                render_init_pieces();

                game_state.stage = 1;
                game_state.is_player1_bot = false;
                game_state.is_player2_bot = false;
            }
            else if (game_state.options[i].command == quit)
            {
                //exit
                game_state.is_quit = true;
            }
            else if (game_state.options[i].command == restart)
            {
                //restart
                game_state.stage = 0;
                game_state.selected_pawn_id = -1;
                game_state.current_colour = 0;
            }
            else if (game_state.options[i].command == resume)
            {
                //resume
                game_state.stage = 1;
            }
        }
    }
}

void game_board_click(int x, int y)
{
    int grid_x = board_x_to_grid(x);
    int grid_y = board_x_to_grid(y);

    Grid selected_grid = { grid_x, grid_y };

    int snapped_x = board_get_snapped_x(x);
    int snapped_y = board_get_snapped_y(y);

    Grid grids[8];

    board_mouse_click(snapped_x, snapped_y);

    for (int i = 0; i < game_state.pawn_count; i++)
    {
        if (game_state.pawns[i].is_active
            && game_state.pawns[i].colour == game_state.current_colour
            && !game_is_current_colour_bot(game_state.current_colour))
        {
            int selected_pawn_id = pawn_mouse_click(&game_state.pawns[i]);

            if (selected_pawn_id > -1)
            {
                Pawn* pawn = pawn_get_by_id(selected_pawn_id);

                pawn_get_moves(pawn, grids);

                Grid current_grid = { game_state.pawns[i].x, game_state.pawns[i].y };

                if (!pawn_is_at_location_grid(&grids[0]))
                {
                    bot_get_grid_rating(selected_pawn_id, game_state.current_colour, current_grid, grids[0]);
                    board_set_movement_tile_grid(0, grids[0]);
                }

                if (!pawn_is_at_location_grid(&grids[1]))
                {
                    bot_get_grid_rating(selected_pawn_id, game_state.current_colour, current_grid, grids[1]);
                    board_set_movement_tile_grid(1, grids[1]);
                }

                if (pawn_is_capture_available(pawn->colour, &grids[0], &grids[2]))
                {
                    bot_get_grid_rating(selected_pawn_id, game_state.current_colour, current_grid, grids[2]);
                    board_set_movement_tile_grid(2, grids[2]);
                }

                if (pawn_is_capture_available(pawn->colour, &grids[1], &grids[3]))
                {
                    bot_get_grid_rating(selected_pawn_id, game_state.current_colour, current_grid, grids[3]);
                    board_set_movement_tile_grid(3, grids[3]);
                }

                if (pawn->is_king)
                {
                    if (!pawn_is_at_location_grid(&grids[4]))
                    {
                        bot_get_grid_rating(selected_pawn_id, game_state.current_colour, current_grid, grids[4]);
                        board_set_movement_tile_grid(4, grids[4]);
                    }

                    if (!pawn_is_at_location_grid(&grids[5]))
                    {
                        bot_get_grid_rating(selected_pawn_id, game_state.current_colour, current_grid, grids[5]);
                        board_set_movement_tile_grid(5, grids[5]);
                    }

                    if (pawn_is_capture_available(pawn->colour, &grids[4], &grids[6]))
                    {
                        bot_get_grid_rating(selected_pawn_id, game_state.current_colour, current_grid, grids[6]);
                        board_set_movement_tile_grid(6, grids[6]);
                    }

                    if (pawn_is_capture_available(pawn->colour, &grids[5], &grids[7]))
                    {
                        bot_get_grid_rating(selected_pawn_id, game_state.current_colour, current_grid, grids[7]);
                        board_set_movement_tile_grid(7, grids[7]);
                    }
                }
            }
        }
    }

    // move selected piece to clicked grid
    if (game_state.selected_pawn_id > -1)
    {
        Pawn* pawn = pawn_get_by_id(game_state.selected_pawn_id);

        pawn_get_moves(pawn, grids);

        bool is_unoccupied = !pawn_is_at_location_grid(&selected_grid);

        bool is_valid_move = pawn_is_valid_move(&selected_grid, &grids[0])
            || pawn_is_valid_move(&selected_grid, &grids[1])
            || (pawn->is_king && pawn_is_valid_move(&selected_grid, &grids[4]))
            || (pawn->is_king && pawn_is_valid_move(&selected_grid, &grids[5]));

        bool is_valid_capture
            = pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[0], &grids[2])
            || pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[1], &grids[3])
            || (pawn->is_king && pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[4], &grids[6]))
            || (pawn->is_king && pawn_is_valid_capture(pawn->colour, &selected_grid, &grids[5], &grids[7]));

        if (pawn != NULL
            && is_unoccupied
            && (is_valid_move || is_valid_capture))
        {
            if (is_valid_capture)
            {
                pawn_capture_move(pawn, &selected_grid);
            }

            int snapped_center_x = board_get_snapped_center_x(x);
            int snapped_center_y = board_get_snapped_center_y(y);

            pawn_set_x(pawn, grid_x, snapped_center_x);
            pawn_set_y(pawn, grid_y, snapped_center_y);

            // switch active player
            game_state.current_colour = game_state.current_colour == 0 ? 1 : 0;

            pawn_deselect_all();
        }
    }

    if (game_state.selected_pawn_id < 0)
    {
        board_clear_tile_grids();
        bot_clear_grid_ratings();
    }
}

void game_mouse_event(int x, int y, Uint32 mouse_state)
{
    if (x > game_state.board_bound_x || y > game_state.board_bound_x) { return; }

    bool is_click = mouse_state == SDL_BUTTON_LEFT;

    bool is_ui = game_state.stage == 0 || game_state.stage == 2 || game_state.stage == 3;

    if (is_ui)
    {
        game_ui_hover(x, y);
    }
    else
    {
        game_mouse_hover(x, y);
    }

    if (!is_click) { return; }

    if (is_ui)
    {
        game_ui_click(x, y);
    }
    else
    {
        game_board_click(x, y);
    }
}