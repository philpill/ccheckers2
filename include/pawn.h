#ifndef PAWN_H
#define PAWN_H

#include <stdbool.h>
#include <SDL.h>
#include "game.h"

void pawn_init(Game *state);
Pawn *pawn_get_all();
void pawn_quit();
void pawn_mouse_event(int x, int y, Uint32 mouse_state);
#endif