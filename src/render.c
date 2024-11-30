#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "render.h"
#include "game.h"

const int RENDER_SCREEN_WIDTH = 640;
const int RENDER_SCREEN_HEIGHT = 480;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

void render_stuff()
{
    int size = 32;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int x = i * size;
            int y = j * size;
            SDL_Rect rect = { x, y, size, size };
            

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            if (SDL_RenderDrawRect(renderer, &rect) < 0)
            {
                printf("SDL_Error: %s\n", SDL_GetError());
            }


            // if ((i + j) % 2 == 0.0) {

            //     SDL_Rect rect2 = { (x + 1), (y + 1), (size - 2), (size - 2) };

            //     SDL_SetRenderDrawColor(renderer, x, 0, y, (x + y) * 4);
            //     if (SDL_RenderFillRect(renderer, &rect2) < 0)
            //     {
            //         printf("SDL_Error: %s\n", SDL_GetError());
            //     }
            // }
        }
    }
}

void render_init()
{
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
        render_stuff();
        SDL_RenderPresent(renderer);
    }
}

void render_quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
