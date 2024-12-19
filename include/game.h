#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <sdl.h>

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
    int size;
    int x;
    int y;
    int colour;
    bool is_enabled;
} Tile;

typedef struct {
    int id;
    int grid_size;
    bool is_quit;
    Pawn *pawns;
    Tile *movement_tiles;
    Tile *cursor_tile;
    Tile *active_tile;
} Game;

void game_init();
Game* game_get_state();
void game_mouse_event(int x, int y, Uint32 mouse_state);
void game_quit();

#endif