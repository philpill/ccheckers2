#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "render.h"

const int RENDER_SCREEN_WIDTH = 640;
const int RENDER_SCREEN_HEIGHT = 480;

static SDL_Window* window = NULL;

//The surface contained by the window
static SDL_Surface* screenSurface = NULL;

void render_init()
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        //Create window
        window = SDL_CreateWindow("crappy checkers 2: SDL edition", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RENDER_SCREEN_WIDTH, RENDER_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface(window);
        }
    }
}

void render_exec()
{
    printf("render_exec()\n\n");

    if (window != NULL && screenSurface != NULL)
    {
        //Fill the surface white
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

        //Update the surface
        SDL_UpdateWindowSurface(window);
    }
}

void render_quit()
{
    //Destroy window
    SDL_DestroyWindow(window);
}
