#ifndef PAWN_H
#define PAWN_H

#include <stdbool.h>
#include <SDL.h>
#include "game.h"

void pawn_init(Game *state);
void pawn_exec();
Pawn *pawn_get_all();
void pawn_quit();
void pawn_mouse_click(Pawn *pawn);
void pawn_mouse_hover(Pawn *pawn, int x, int y);
Pawn* pawn_get_by_id(int id);
#endif