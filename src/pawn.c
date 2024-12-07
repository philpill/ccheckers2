#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pawn.h"

static Pawn* pawns_ptr;

Pawn* pawn_get_all()
{
    return pawns_ptr;
}

void pawn_init()
{
    pawns_ptr = malloc(24 * sizeof(Pawn));

    int id = 0;
    for (int i = 0; i < 3; i++)
    {
        int start = i % 2 > 0 ? 1 : 0;
        for (int j = start; j < 8;)
        {
            pawns_ptr[id].id = id;
            pawns_ptr[id].colour = 0;
            pawns_ptr[id].x = j;
            pawns_ptr[id].y = i;
            pawns_ptr[id].is_king = false;
            pawns_ptr[id].is_hover = false;

            id++;
            j = j + 2;
        }
    }

    for (int i = 5; i < 8; i++)
    {
        int start = i % 2 > 0 ? 1 : 0;
        for (int j = start; j < 8;)
        {
            pawns_ptr[id].id = id;
            pawns_ptr[id].colour = 1;
            pawns_ptr[id].x = j;
            pawns_ptr[id].y = i;
            pawns_ptr[id].is_king = false;
            pawns_ptr[id].is_hover = false;

            id++;
            j = j + 2;
        }
    }
}

void pawn_mouse_event(int x, int y, Uint32 mouse_state)
{
    // printf("pawn_mouse_event x: %d, y: %d, mouse: %d\n", x, y, mouse_state);

    int radius = 10;

    for (int i = 0; i < 24; i++)
    {
        double dist = abs(x - pawns_ptr[i].grid_x) + abs(y - pawns_ptr[i].grid_y);

        pawns_ptr[i].is_hover = (dist <= radius);

        // if (pawns_ptr[i].is_hover) {
        //     printf("pawwn: %d\n", pawns_ptr[i].id);
        // }
    }
}

void pawn_quit()
{
    free(pawns_ptr);
}

