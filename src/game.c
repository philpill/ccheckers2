#include <stdlib.h>
#include <sdl.h>
#include <stdio.h>
#include "game.h"
#include "pawn.h"
#include "board.h"
#include "render.h"

static Game game_state;

void game_init()
{
    game_state.id = 1;
    game_state.board_offset_x = 10;
    game_state.board_offset_y = 10;
    game_state.current_colour = 0;
    game_state.selected_pawn_id = -1;
    game_state.pawn_count = 24;
    game_state.is_quit = false;
    game_state.pawns = malloc(24 * sizeof(Pawn));

    game_state.movement_tiles = malloc(4 * sizeof(Tile));
    game_state.active_tile = malloc(sizeof(Tile));
    game_state.cursor_tile = malloc(sizeof(Tile));
}

Game* game_get_state()
{
    return &game_state;
}

void game_quit()
{
    free(game_state.pawns);
    free(game_state.movement_tiles);
    free(game_state.active_tile);
    free(game_state.cursor_tile);
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

void game_mouse_click(int x, int y)
{
    int grid_x = board_x_to_grid(x);
    int grid_y = board_x_to_grid(y);

    Grid selected_grid = { grid_x, grid_y };

    int snapped_x = board_get_snapped_x(x);
    int snapped_y = board_get_snapped_y(y);

    Grid grid1 = { -1, -1 };
    Grid grid2 = { -1, -1 };
    Grid grid3 = { -1, -1 };
    Grid grid4 = { -1, -1 };

    board_mouse_click(snapped_x, snapped_y);

    for (int i = 0; i < game_state.pawn_count; i++)
    {
        if (game_state.pawns[i].is_active 
            && game_state.pawns[i].colour == game_state.current_colour)
        {
            int selected_pawn_id = pawn_mouse_click(&game_state.pawns[i]);

            if (selected_pawn_id > -1)
            {
                // printf("id: %d\n", selected_pawn_id);

                Pawn* pawn = pawn_get_by_id(selected_pawn_id);

                pawn_get_moves(pawn, &grid1, &grid2, &grid3, &grid4);

                if (!pawn_is_at_location_grid(&grid1))
                {
                    board_set_movement_tile_grid(0, grid1);
                }

                if (!pawn_is_at_location_grid(&grid2))
                {
                    board_set_movement_tile_grid(1, grid2);
                }

                if (pawn_is_capture_available(pawn->colour, &grid1, &grid3))
                {
                    board_set_movement_tile_grid(2, grid3);
                }

                if (pawn_is_capture_available(pawn->colour, &grid2, &grid4))
                {
                    board_set_movement_tile_grid(3, grid4);
                }
            }
        }
    }

    // move selected piece to clicked grid
    if (game_state.selected_pawn_id > -1)
    {
        // printf("id: %d\n", game_state.selected_pawn_id);

        Pawn* pawn = pawn_get_by_id(game_state.selected_pawn_id);

        pawn_get_moves(pawn, &grid1, &grid2, &grid3, &grid4);

        bool is_unoccupied = !pawn_is_at_location_grid(&selected_grid);

        bool is_valid_move = pawn_is_valid_move(&selected_grid, &grid1)
            || pawn_is_valid_move(&selected_grid, &grid2);

        bool is_valid_capture 
            = pawn_is_valid_capture(pawn->colour, &selected_grid, &grid1, &grid3)
            || pawn_is_valid_capture(pawn->colour, &selected_grid, &grid2, &grid4);

        // printf("\n2---------------\n");
        // printf("3 grid_x: %d, grid_y: %d\n", grid_x, grid_y);
        // printf("1 %d, %d\n", grid1.x, grid1.y);
        // printf("2 %d, %d\n", grid2.x, grid2.y);
        // printf("3 %d, %d\n", grid3.x, grid3.y);
        // printf("4 %d, %d\n", grid4.x, grid4.y);
        // printf("\n2---------------\n");

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
    }
}

void game_mouse_event(int x, int y, Uint32 mouse_state)
{
    bool is_click = mouse_state == SDL_BUTTON_LEFT;

    if (x > game_state.board_bound_x || y > game_state.board_bound_x) { return; }

    game_mouse_hover(x, y);

    if (!is_click) { return; }

    game_mouse_click(x, y);
}