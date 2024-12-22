#ifndef PAWN_H
#define PAWN_H

#include <stdbool.h>
#include <SDL.h>
#include "game.h"

bool pawn_is_at_location(int x, int y);
Pawn *pawn_get_all();
Pawn* pawn_get_by_id(int id);
void pawn_exec();
void pawn_init(Game *state);
void pawn_mouse_click(Pawn *pawn);
void pawn_mouse_hover(Pawn *pawn, int x, int y);
void pawn_quit();
void pawn_set_x(Pawn *pawn, int x);
void pawn_set_y(Pawn *pawn, int y);
#endif