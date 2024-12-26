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
        board_mouse_hover(x, y);

        if (is_click)
        {
            board_mouse_click(x, y);

            if (game_state.selected_pawn_id > -1)
            {
                Pawn* pawn = pawn_get_by_id(game_state.selected_pawn_id);

                int snapped_x = board_get_snapped_center_x(x);
                int snapped_y = board_get_snapped_center_y(y);

                if (pawn != NULL && !pawn_is_at_location(snapped_x, snapped_y))
                {
                    pawn_set_x(pawn, snapped_x);
                    pawn_set_y(pawn, snapped_y);
                }
            }
        }

        for (int i = 0; i < game_state.pawn_count; i++)
        {
            if (game_state.pawns[i].is_active)
            {
                pawn_mouse_hover(&game_state.pawns[i], x, y);

                if (is_click)
                {
                    pawn_mouse_click(&game_state.pawns[i]);

                    if (game_state.selected_pawn_id > -1)
                    {
                        Grid grid1 = { 0, 0 };
                        Grid grid2 = { 0, 0 };
                        Grid grid3 = { 0, 0 };
                        Grid grid4 = { 0, 0 };

                        Pawn *pawn = pawn_get_by_id(game_state.selected_pawn_id);

                        pawn_get_moves(pawn, &grid1, &grid2, &grid3, &grid4);

                        printf("grid1: %d %d\n", grid1.x, grid1.y);
                        printf("grid2: %d %d\n", grid2.x, grid2.y);
                        printf("grid3: %d %d\n", grid3.x, grid3.y);
                        printf("grid4: %d %d\n", grid4.x, grid4.y);

                        board_set_movement_tile_grid(0, grid1);
                        board_set_movement_tile_grid(1, grid2);
                    }
                }
            }
        }
    }
}