#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "render.h"
#include "game.h"
#include "utils.h"
#include "pawn.h"

const int RENDER_SCREEN_WIDTH = 640;
const int RENDER_SCREEN_HEIGHT = 480;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static int grid_size;

static Game* game_state;

void render_piece(Pawn* pawn)
{
    if (pawn->is_selected)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }
    else if (pawn->is_hover)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

    int error = pawn->colour == 0
        ? SDL_RenderFillCircle(renderer, pawn->grid_x, pawn->grid_y, pawn->radius)
        : SDL_RenderDrawCircle(renderer, pawn->grid_x, pawn->grid_y, pawn->radius);
    if (error < 0)
    {
        printf("error: %s\n", SDL_GetError());
    }
}

void render_tile(Tile* tile)
{
    SDL_Rect fillRect = {
        tile->x + 1,
        tile->y + 1,
        game_state->grid_size - 1,
        game_state->grid_size - 1
    };
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &fillRect);
}

void render_pieces()
{
    Pawn* pawns = pawn_get_all();
    for (int i = 0; i < 24; i++)
    {
        render_piece(&pawns[i]);
    }
}

void render_tiles()
{
    render_tile(game_state->cursor_tile);
}

void render_board()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 9; i++)
    {
        if (SDL_RenderDrawLine(renderer,
            game_state->board_offset_x,
            game_state->board_offset_y + (grid_size * i),
            game_state->board_offset_x + (grid_size * 8),
            game_state->board_offset_y + (grid_size * i)) < 0)
        {
            printf("error: %s\n", SDL_GetError());
        }

        if (SDL_RenderDrawLine(renderer,
            game_state->board_offset_x + (grid_size * i),
            game_state->board_offset_y,
            game_state->board_offset_x + (grid_size * i),
            game_state->board_offset_y + (grid_size * 8)) < 0)
        {
            printf("error: %s\n", SDL_GetError());
        }
    }
}

void init_pieces()
{
    int radius = (grid_size / 2) - 6; //10
    Pawn* pawns = pawn_get_all();
    for (int i = 0; i < 24; i++)
    {
        pawns[i].radius = radius;
        pawns[i].grid_x = (pawns[i].x * grid_size) 
            + (pawns[i].radius / 2) 
            + pawns[i].radius 
            + game_state->board_offset_x;
        pawns[i].grid_y = (pawns[i].y * grid_size) 
            + (pawns[i].radius / 2) 
            + pawns[i].radius 
            + game_state->board_offset_y;
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

    grid_size = state->grid_size;

    init_pieces();
}

void render_exec(Game* state)
{
    if (window != NULL)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_board();
        render_tiles();
        render_pieces();
        SDL_RenderPresent(renderer);
    }
}

void render_quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
