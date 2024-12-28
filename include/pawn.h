#ifndef PAWN_H
#define PAWN_H

#include <stdbool.h>
#include <SDL.h>
#include "game.h"

bool pawn_is_at_location(int x, int y);
bool pawn_is_at_location_grid(int grid_x, int grid_y);
bool pawn_mouse_hover_by_grid(Pawn* pawn, int grid_x, int grid_y);
Pawn *pawn_get_all();
Pawn* pawn_get_by_id(int id);
void pawn_deselect_all();
void pawn_exec();
void pawn_get_moves(Pawn* pawn, Grid* grid_1, Grid* grid_2, Grid* grid_3, Grid* grid_4);
void pawn_init(Game *state);
int pawn_mouse_click(Pawn *pawn);
void pawn_mouse_hover(Pawn *pawn, int x, int y);
void pawn_quit();
void pawn_set_x(Pawn *pawn, int grid_x, int x);
void pawn_set_y(Pawn *pawn, int grid_y, int y);
#endif