#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <sdl.h>
#include <stdio.h>

typedef struct {
    int id;
    int colour;
    float x;
    float y;
    int dest_x;
    int dest_y;
    int grid_x;
    int grid_y;
    int radius;
    int direction;
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

enum option_command { onevcpu, onevtwo, resume, restart, quit };

typedef struct {
    bool is_hover;
    char* text;
    enum option_command command;
    int colour_index;
    int group;
    int height;
    int id;
    int width;
    int x1;
    int x2;
    int y1;
    int y2;
} Option;

typedef struct {
    int id;
    int grid_size;
    int board_offset_x;
    int board_offset_y;
    int board_bound_x;
    int board_bound_y;
    int current_colour;
    int selected_pawn_id;
    int pawn_count;
    int selected_option_index;
    int stage;
    float animation_speed;
    bool is_quit;
    bool is_player1_bot;
    bool is_player2_bot;
    bool is_animating;
    Option* options;
    Pawn* pawns;
    Tile* movement_tiles;
    Tile* cursor_tile;
    Tile* active_tile;
    Uint64 step;
} Game;

typedef struct {
    int x;
    int y;
} Grid;

typedef struct {
    int rating_id;
    int pawn_id;
    int grid_index;
    int rating;
} MaxRating;

bool game_is_valid_move(int pawn_id, int grid_x, int grid_y);
bool game_is_valid_capture(int pawn_id, int grid_x, int grid_y);
void game_init();
Game* game_get_state();
void game_mouse_event(int x, int y, Uint32 mouse_state);
void game_quit();
void game_start();

#endif