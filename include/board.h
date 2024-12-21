#ifndef BOARD_H
#define BOARD_H

#include "game.h"

void board_init();
void board_exec(Game *state);
int board_get_snapped_x(int x);
int board_get_snapped_y(int y);
void board_mouse_hover(int x, int y);
void board_mouse_click(int x, int y);
void board_quit();

#endif