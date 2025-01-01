#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "render.h"
#include "game.h"
#include "utils.h"
#include "pawn.h"
#include "board.h"

const int RENDER_SCREEN_WIDTH = 640;
const int RENDER_SCREEN_HEIGHT = 480;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static Game* game_state;

// 0 white
// 1 black
// 2 red
// 3 green
// 4 blue
// 5 grey
static void set_render_colour(int index)
{
    switch (index)
    {
    case 0:
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        break;
    case 1:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        break;
    case 2:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;
    case 3:
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        break;
    case 4:
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        break;
    case 5:
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        break;
    default:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
}

void render_piece(Pawn* pawn)
{
    if (pawn->is_selected)
    {
        set_render_colour(3);
    }
    else if (pawn->is_hover)
    {
        set_render_colour(2);
    }
    else
    {
        set_render_colour(0);
    }

    int error = pawn->colour == 0
        ? SDL_RenderFillCircle(renderer, pawn->grid_x, pawn->grid_y, pawn->radius)
        : SDL_RenderDrawCircle(renderer, pawn->grid_x, pawn->grid_y, pawn->radius);
    if (error < 0)
    {
        printf("error: %s\n", SDL_GetError());
    }
}

void render_ui()
{
    SDL_Rect fill_rect = {
        game_state->board_bound_x + 11,
        game_state->board_offset_y + 1,
        game_state->grid_size - 2,
        game_state->grid_size - 2
    };

    SDL_Rect outline_rect = {
        game_state->board_bound_x + 10,
        game_state->board_offset_y,
        game_state->grid_size,
        game_state->grid_size
    };

    set_render_colour(5);
    SDL_RenderFillRect(renderer, &outline_rect);

    set_render_colour(game_state->current_colour);
    SDL_RenderFillRect(renderer, &fill_rect);
}

void render_tile(Tile* tile)
{
    if (tile->is_enabled)
    {
        SDL_Rect fillRect = {
            tile->x + 1,
            tile->y + 1,
            game_state->grid_size - 1,
            game_state->grid_size - 1
        };

        set_render_colour(tile->colour);
        SDL_RenderFillRect(renderer, &fillRect);
    }
}

void render_pieces()
{
    Pawn* pawns = pawn_get_all();
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        render_piece(&pawns[i]);
    }
}

void render_tiles()
{
    render_tile(game_state->active_tile);
    render_tile(game_state->cursor_tile);
    for (int i = 0; i < 4; i++)
    {
        render_tile(&game_state->movement_tiles[i]);
    }
}

void render_board()
{
    set_render_colour(0);

    for (int i = 0; i < 9; i++)
    {
        if (SDL_RenderDrawLine(renderer,
            game_state->board_offset_x,
            game_state->board_offset_y + (game_state->grid_size * i),
            game_state->board_offset_x + (game_state->grid_size * 8),
            game_state->board_offset_y + (game_state->grid_size * i)) < 0)
        {
            printf("error: %s\n", SDL_GetError());
        }

        if (SDL_RenderDrawLine(renderer,
            game_state->board_offset_x + (game_state->grid_size * i),
            game_state->board_offset_y,
            game_state->board_offset_x + (game_state->grid_size * i),
            game_state->board_offset_y + (game_state->grid_size * 8)) < 0)
        {
            printf("error: %s\n", SDL_GetError());
        }
    }
}

void render_update_grid_x(Pawn* pawn)
{
    int radius = (game_state->grid_size / 2) - 6; //10
    pawn->radius = radius;
    int grid_x = (pawn->x * game_state->grid_size) + game_state->board_offset_x;
    pawn->grid_x = board_get_snapped_center_x(grid_x);
}

void render_update_grid_y(Pawn* pawn)
{
    int radius = (game_state->grid_size / 2) - 6; //10
    pawn->radius = radius;
    int grid_y = (pawn->y * game_state->grid_size) + game_state->board_offset_y;
    pawn->grid_y = board_get_snapped_center_y(grid_y);
}

void init_pieces()
{
    Pawn* pawns = pawn_get_all();
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        render_update_grid_x(&pawns[i]);
        render_update_grid_y(&pawns[i]);
    }
}

void render_init(Game* state)
{
    game_state = state;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("error: %s\n", SDL_GetError());
    }

    window = SDL_CreateWindow("crappy checkers 2: SDL edition",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        RENDER_SCREEN_WIDTH,
        RENDER_SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        printf("error: %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
    {
        printf("error: %s\n", SDL_GetError());
    }

    init_pieces();
}

void render_exec(Game* state)
{
    if (window != NULL)
    {
        set_render_colour(1);
        SDL_RenderClear(renderer);
        render_board();
        render_tiles();
        render_pieces();
        render_ui();
        SDL_RenderPresent(renderer);
    }
}

void render_quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
