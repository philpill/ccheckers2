#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef struct {
    int id;
    int colour;
    int x;
    int y;
    int grid_x;
    int grid_y;
    int radius;
    bool is_king;
    bool is_hover;
    bool is_selected;
    bool is_active;
} Pawn;

typedef struct {
    int id;
    bool is_quit;
    Pawn *pawns;
} Game;

void game_init();
Game* game_get_state();
void game_quit();

#endif