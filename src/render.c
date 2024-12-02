#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "render.h"
#include "game.h"
#include "utils.h"

const int RENDER_SCREEN_WIDTH = 640;
const int RENDER_SCREEN_HEIGHT = 480;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static int grid_size = 32; // 16
static int radius = 0;

void render_piece(int x, int y, bool isFilled)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int circle_x = (x * grid_size) + 5 + radius;
    int circle_y = (y * grid_size) + 5 + radius;
    int error = isFilled
        ? SDL_RenderFillCircle(renderer, circle_x, circle_y, radius)
        : SDL_RenderDrawCircle(renderer, circle_x, circle_y, radius);
    if (error < 0)
    {
        printf("SDL_Error: %s\n", SDL_GetError());
    }
}

void render_pieces()
{
    for (int i = 0; i < 3; i++)
    {
        int start = i % 2 > 0 ? 1 : 0;
        for (int j = start; j < 8;)
        {
            render_piece(j, i, true);
            j = j + 2;
        }
    }

    for (int i = 5; i < 8; i++)
    {
        int start = i % 2 > 0 ? 1 : 0;
        for (int j = start; j < 8;)
        {
            render_piece(j, i, false);
            j = j + 2;
        }
    }
}

void render_board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int x = i * grid_size;
            int y = j * grid_size;
            SDL_Rect rect = { x, y, grid_size, grid_size };

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            if (SDL_RenderDrawRect(renderer, &rect) < 0)
            {
                printf("SDL_Error: %s\n", SDL_GetError());
            }
        }
    }
}

void render_init()
{
    radius = (grid_size / 2) - 6; //10

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("crappy checkers 2: SDL edition", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RENDER_SCREEN_WIDTH, RENDER_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if (renderer == NULL)
            {
                printf("SDL_Error: %s\n", SDL_GetError());
            }
        }
    }
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
