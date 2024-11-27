#include <SDL.h>
#include <SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include "render.h"

static Uint64 frame_start = 0;
static Uint64 frame_end = 0;
static int fps = 30;
static bool is_quit = false;

void loop()
{
    render_exec();
}

int main(int argc, char* args[])
{
    printf("main()\n");

    double interval = 1.0 / fps * 1000.0;

    printf("interval %f", interval);

    SDL_Event e;

    render_init();

    while (!is_quit)
    {
        frame_start = SDL_GetTicks64();

        // printf("start %d\n", frame_start);

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {

            // printf("%d\n", e.type);
            // printf("%d\n", e.key.keysym.sym);

            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                is_quit = true;
                break;
            }

            switch (e.type)
            {
            case SDL_QUIT:
                is_quit = true;
                break;
            }
        }

        loop();

        frame_end = SDL_GetTicks64();
        // printf("end %d\n", frame_end);

        if (frame_start - frame_end > interval)
        {
            SDL_Delay(frame_start + interval - frame_end);
        }
    }

    render_quit();

    SDL_Quit();

    return 0;
}