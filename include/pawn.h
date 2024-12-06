#ifndef PAWN_H
#define PAWN_H

#include <stdbool.h>
#include <SDL.h>

typedef struct {
    int id;
    int colour;
    int x;
    int y;
    int grid_x;
    int grid_y;
    bool is_king;
    bool is_hover;
} Pawn;

void pawn_init();
Pawn *pawn_get_all();
void pawn_quit();
void pawn_mouse_event(int x, int y, Uint32 mouse_state);
#endif