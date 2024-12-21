#ifndef RENDER_H
#define RENDER_H

#include "game.h"

void render_init(Game *state);
void render_exec();
void render_update_grid_x(Pawn* pawn);
void render_update_grid_y(Pawn* pawn);
void render_quit();

#endif
