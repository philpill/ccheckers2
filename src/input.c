#include <SDL.h>
#include "game.h"
#include "input.h"

void input_exec(Game* state)
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            state->is_quit = true;
            break;
        }

        switch (e.type)
        {
        case SDL_QUIT:
            state->is_quit = true;
            break;
        }
    }
}