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

void game_mouse_event(int x, int y, Uint32 mouse_state)
{
    bool is_click = mouse_state == SDL_BUTTON_LEFT;

    if (x < game_state.board_bound_x && y < game_state.board_bound_x)
    {
        int grid_x = board_x_to_grid(x);
        int grid_y = board_x_to_grid(y);

        int snapped_x = board_get_snapped_x(x);
        int snapped_y = board_get_snapped_y(y);

        board_mouse_hover(snapped_x, snapped_y);

        for (int i = 0; i < game_state.pawn_count; i++)
        {
            if (game_state.pawns[i].is_active)
            {
                pawn_mouse_hover_by_grid(&game_state.pawns[i], grid_x, grid_y);
            }
        }

        if (is_click)
        {
            Grid grid1 = { -1, -1 };
            Grid grid2 = { -1, -1 };
            Grid grid3 = { -1, -1 };
            Grid grid4 = { -1, -1 };

            board_mouse_click(snapped_x, snapped_y);

            for (int i = 0; i < game_state.pawn_count; i++)
            {
                if (game_state.pawns[i].is_active)
                {
                    board_clear_tile_grids();
                    pawn_mouse_click(&game_state.pawns[i]);

                    if (game_state.selected_pawn_id > -1)
                    {
                        Pawn* pawn = pawn_get_by_id(game_state.selected_pawn_id);

                        pawn_get_moves(pawn, &grid1, &grid2, &grid3, &grid4);

                        if (!pawn_is_at_location_grid(grid1.x, grid1.y))
                        {
                            board_set_movement_tile_grid(0, grid1);
                        }

                        if (!pawn_is_at_location_grid(grid2.x, grid2.y))
                        {
                            board_set_movement_tile_grid(1, grid2);
                        }
                    }
                }
            }

            if (game_state.selected_pawn_id > -1)
            {
                printf("TEST\n");

                Pawn* pawn = pawn_get_by_id(game_state.selected_pawn_id);

                if (pawn != NULL
                    && !pawn_is_at_location_grid(grid_x, grid_y)
                    && ((grid_x == grid1.x && grid_y == grid1.y)
                        || (grid_x == grid2.x && grid_y == grid2.y)
                        || (grid_x == grid3.x && grid_y == grid3.y)
                        || (grid_x == grid4.x && grid_y == grid4.y)
                        )
                    )
                {
                    // printf("2 grid_x: %d, grid_y: %d\n", grid_x, grid_y);

                    int snapped_center_x = board_get_snapped_center_x(x);
                    int snapped_center_y = board_get_snapped_center_y(y);

                    pawn_set_x(pawn, grid_x, snapped_center_x);
                    pawn_set_y(pawn, grid_y, snapped_center_y);
                    pawn_deselect_all();
                    board_clear_tile_grids();
                }
            }
        }
    }
}