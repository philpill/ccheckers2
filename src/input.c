#include <SDL.h>
#include <stdio.h>
#include "game.h"
#include "input.h"

void input_exec(Game* state)
{
    SDL_Event e;

    int x, y, mouse_state;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                state->stage = state->stage == 3 ? 1 : 3;
                break;
            }
        }
        else
        {
            switch (e.type)
            {
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            // case SDL_MOUSEBUTTONUP:
                mouse_state = SDL_GetMouseState(&x, &y);
                // printf("x: %d, y: %d, mouse: %d\n", x, y, mouse_state);
                game_mouse_event(x, y, mouse_state);
                break;
            case SDL_QUIT:
                state->is_quit = true;
                break;
            }
        }
    }
}