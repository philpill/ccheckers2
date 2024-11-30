#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef struct {
    int id;
    bool is_quit;
} Game;

void game_init();
Game* game_get_state();
void game_quit();

#endif