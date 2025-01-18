#ifndef BOT_H
#define BOT_H

#include <stdbool.h>
#include <SDL.h>
#include "game.h"

int bot_get_grid_rating(int pawn_id, int colour, Grid current_grid, Grid dest_grid);
void bot_clear_grid_ratings();
void bot_exec();
void bot_init(Game *state);
void bot_quit();

#endif