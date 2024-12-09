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

static int grid_size = 32; // 16

static int offset_x = 10;
static int offset_y = 10;

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

void render_pieces()
{
    Pawn* pawns = pawn_get_all();
    for (int i = 0; i < 24; i++)
    {
        render_piece(&pawns[i]);
    }
}

void render_board()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 9; i++)
    {
        if (SDL_RenderDrawLine(renderer,
            offset_x,
            offset_y + (grid_size * i),
            offset_x + (grid_size * 8),
            offset_y + (grid_size * i)) < 0)
        {
            printf("error: %s\n", SDL_GetError());
        }

        if (SDL_RenderDrawLine(renderer,
            offset_x + (grid_size * i),
            offset_y,
            offset_x + (grid_size * i),
            offset_y + (grid_size * 8)) < 0)
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
        pawns[i].grid_x = (pawns[i].x * grid_size) + (pawns[i].radius / 2) + pawns[i].radius + offset_x;
        pawns[i].grid_y = (pawns[i].y * grid_size) + (pawns[i].radius / 2) + pawns[i].radius + offset_y;
    }
}

void render_init(Game *state)
{
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
        render_board();
        render_pieces();
        SDL_RenderPresent(renderer);
    }
}

void render_quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
