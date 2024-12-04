#ifndef PAWN_H
#define PAWN_H

#include <stdbool.h>

typedef struct {
    int id;
    int colour;
    int x;
    int y;
    bool is_king;
} Pawn;

void pawn_init();
Pawn *pawn_get_all();
void pawn_quit();

#endif