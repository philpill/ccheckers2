#ifndef PAWN_H
#define PAWN_H

#include <stdbool.h>
#include <SDL.h>
#include "game.h"

bool pawn_is_at_location_grid(Grid *grid);
bool pawn_is_capture_available(int pawn_colour, Grid *grid1, Grid *grid2);
bool pawn_is_opposition(int colour, Grid *grid);
bool pawn_is_valid_capture(int pawn_colour, Grid *selected_grid, Grid *grid1, Grid *grid2);
bool pawn_is_valid_move(Grid* selected_grid, Grid* grid1);
bool pawn_mouse_hover_by_grid(Pawn* pawn, int grid_x, int grid_y);
int pawn_mouse_click(Pawn *pawn);
Pawn *pawn_get_all();
Pawn* pawn_get_by_id(int id);
void pawn_capture_move(Pawn *pawn, Grid *grid);
void pawn_capture(Pawn* pawn);
void pawn_deselect_all();
void pawn_exec();
void pawn_get_moves(Pawn* pawn, Grid* grids);
void pawn_init(Game *state);
void pawn_initialise_pawns();
void pawn_initialise_pawns_test();
void pawn_mouse_hover(Pawn *pawn, int x, int y);
void pawn_quit();
void pawn_set_x(Pawn *pawn, int grid_x, int x);
void pawn_set_y(Pawn *pawn, int grid_y, int y);

#endif