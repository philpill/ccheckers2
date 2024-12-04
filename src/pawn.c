#include <stdio.h>
#include <stdlib.h>
#include "pawn.h"

static Pawn *pawns_ptr;

Pawn *pawn_get_all()
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

            // printf("%d %d %d\n", pawns_ptr[i].id, pawns_ptr[i].x, pawns_ptr[i].y);

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

            // printf("%d %d %d\n", pawns_ptr[i].id, pawns_ptr[i].x, pawns_ptr[i].y);
            
            id++;
            j = j + 2;
        }
    }
}


void pawn_quit()
{
    free(pawns_ptr);
}

