#include <math.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "render.h"
#include "game.h"
#include "utils.h"
#include "pawn.h"
#include "board.h"

const int RENDER_SCREEN_WIDTH = 640;
const int RENDER_SCREEN_HEIGHT = 480;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* font = NULL;

static Game* game_state;

// 0 white
// 1 black
// 2 red
// 3 green
// 4 blue
// 5 grey
static void set_render_colour(int index)
{
    switch (index)
    {
    case 0:
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        break;
    case 1:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        break;
    case 2:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;
    case 3:
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        break;
    case 4:
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        break;
    case 5:
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        break;
    default:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
}

void render_piece(Pawn* pawn)
{
    if (!pawn->is_active) { return; }

    if (pawn->is_selected)
    {
        set_render_colour(3);
    }
    else if (pawn->is_hover)
    {
        set_render_colour(2);
    }
    else
    {
        set_render_colour(0);
    }

    int error = pawn->colour == 0
        ? SDL_RenderFillCircle(renderer, pawn->grid_x, pawn->grid_y, pawn->radius)
        : SDL_RenderDrawCircle(renderer, pawn->grid_x, pawn->grid_y, pawn->radius);
    if (error < 0)
    {
        printf("error: %s\n", SDL_GetError());
    }

    if (pawn->is_king)
    {
        double a = 5.50; // 315;
        double b = 0.79; // 45;
        double c = 2.36; // 135;
        double d = 3.93; // 225;

        if (pawn->is_selected)
        {
            set_render_colour(pawn->colour == 0 ? 1 : 3);
        }
        else if (pawn->is_hover)
        {
            set_render_colour(pawn->colour == 0 ? 1 : 2);
        }
        else
        {
            set_render_colour(pawn->colour == 0 ? 1 : 0);
        }

        Grid grid1 =
        {
            (pawn->radius * sin(a) + pawn->grid_x),
            (pawn->radius * cos(a) + pawn->grid_y)
        };

        Grid grid2 =
        {
            (pawn->radius * sin(b) + pawn->grid_x),
            (pawn->radius * cos(b) + pawn->grid_y)
        };

        Grid grid3 =
        {
            (pawn->radius * sin(c) + pawn->grid_x),
            (pawn->radius * cos(c) + pawn->grid_y)
        };

        Grid grid4 =
        {
            (pawn->radius * sin(d) + pawn->grid_x),
            (pawn->radius * cos(d) + pawn->grid_y)
        };

        if (SDL_RenderDrawLine(renderer,
            grid1.x, grid1.y,
            grid3.x, grid3.y) < 0)
        {
            printf("error: %s\n", SDL_GetError());
        }

        if (SDL_RenderDrawLine(renderer,
            grid2.x, grid2.y,
            grid4.x, grid4.y) < 0)
        {
            printf("error: %s\n", SDL_GetError());
        }
    }
}

void render_dialog_frame()
{
    SDL_Rect fill_rect = {
        game_state->board_offset_x + 20,
        game_state->board_offset_y + 20,
        (game_state->grid_size * 8) - 40 - 2,
        (game_state->grid_size * 8) - 40 - 2
    };

    SDL_Rect outline_rect = {
        game_state->board_offset_x + 19,
        game_state->board_offset_y + 19,
        (game_state->grid_size * 8) - 40,
        (game_state->grid_size * 8) - 40
    };

    set_render_colour(0);
    if (SDL_RenderFillRect(renderer, &outline_rect) < 0)
    {
        printf("error: %s\n", SDL_GetError());
    }

    set_render_colour(1);
    if (SDL_RenderFillRect(renderer, &fill_rect) < 0)
    {
        printf("error: %s\n", SDL_GetError());
    }
}

void render_option(Option* option)
{
    SDL_Color colour_white = { 255, 255, 255 };
    SDL_Color colour_grey = { 100, 100, 100 };

    SDL_Color option_colour = option->colour_index == 1
        ? colour_white 
        : colour_grey;

    SDL_Surface* surface = TTF_RenderText_Solid(
        font, 
        option->text, 
        option_colour
    );

    int w, h;

    TTF_SizeText(font, option->text, &w, &h);

    option->x2 = option->x1 + w;
    option->y2 = option->y1 + h;

    if (surface == NULL)
    {
        printf("error: %s\n", SDL_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL)
    {
        printf("error: %s\n", SDL_GetError());
    }

    SDL_Rect rect; //create a rect
    rect.x = option->x1;  //controls the rect's x coordinate 
    rect.y = option->y1; // controls the rect's y coordinte
    rect.w = w; // controls the width of the rect
    rect.h = h; // controls the height of the rect

    if (SDL_RenderCopy(renderer, texture, NULL, &rect) < 0)
    {
        printf("error: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render_options(int group)
{
    if (group == 0)
    {
        render_option(&(game_state->options[0]));
        render_option(&(game_state->options[1]));
        render_option(&(game_state->options[2]));
    }
    if (group == 1)
    {
        render_option(&(game_state->options[3]));
        render_option(&(game_state->options[4]));
    }
    if (group == 2)
    {
        render_option(&(game_state->options[5]));
        render_option(&(game_state->options[6]));
        render_option(&(game_state->options[7]));
    }
}

void render_stage_0()
{
    render_dialog_frame();
    render_options(0);
}

void render_stage_1()
{
    SDL_Rect fill_rect = {
        game_state->board_bound_x + 11,
        game_state->board_offset_y + 1,
        game_state->grid_size - 2,
        game_state->grid_size - 2
    };

    SDL_Rect outline_rect = {
        game_state->board_bound_x + 10,
        game_state->board_offset_y,
        game_state->grid_size,
        game_state->grid_size
    };

    set_render_colour(5);
    SDL_RenderFillRect(renderer, &outline_rect);

    set_render_colour(game_state->current_colour);
    SDL_RenderFillRect(renderer, &fill_rect);
}

void render_stage_2()
{
    render_dialog_frame();
    render_options(1);
}

void render_stage_3()
{
    render_dialog_frame();
    render_options(2);
}

void render_ui()
{
    switch (game_state->stage)
    {
    case 0:
        render_stage_0();
        break;
    case 1:
        render_stage_1();
        break;
    case 2:
        render_stage_2();
        break;
    case 3:
        render_stage_3();
        break;
    }
}

void render_tile(Tile* tile)
{
    if (tile->is_enabled)
    {
        SDL_Rect fillRect = {
            tile->x + 1,
            tile->y + 1,
            game_state->grid_size - 1,
            game_state->grid_size - 1
        };

        set_render_colour(tile->colour);
        SDL_RenderFillRect(renderer, &fillRect);
    }
}

void render_pieces()
{
    Pawn* pawns = pawn_get_all();
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        render_piece(&pawns[i]);
    }
}

void render_tiles()
{
    render_tile(game_state->active_tile);
    render_tile(game_state->cursor_tile);
    for (int i = 0; i < 8; i++)
    {
        render_tile(&game_state->movement_tiles[i]);
    }
}

void render_board()
{
    set_render_colour(0);

    for (int i = 0; i < 9; i++)
    {
        if (SDL_RenderDrawLine(renderer,
            game_state->board_offset_x,
            game_state->board_offset_y + (game_state->grid_size * i),
            game_state->board_offset_x + (game_state->grid_size * 8),
            game_state->board_offset_y + (game_state->grid_size * i)) < 0)
        {
            printf("error: %s\n", SDL_GetError());
        }

        if (SDL_RenderDrawLine(renderer,
            game_state->board_offset_x + (game_state->grid_size * i),
            game_state->board_offset_y,
            game_state->board_offset_x + (game_state->grid_size * i),
            game_state->board_offset_y + (game_state->grid_size * 8)) < 0)
        {
            printf("error: %s\n", SDL_GetError());
        }
    }
}

void render_update_grid_x(Pawn* pawn)
{
    int radius = (game_state->grid_size / 2) - 6; //10
    pawn->radius = radius;
    int grid_x = (pawn->x * game_state->grid_size) + game_state->board_offset_x;
    pawn->grid_x = board_get_snapped_center_x(grid_x);
}

void render_update_grid_y(Pawn* pawn)
{
    int radius = (game_state->grid_size / 2) - 6; //10
    pawn->radius = radius;
    int grid_y = (pawn->y * game_state->grid_size) + game_state->board_offset_y;
    pawn->grid_y = board_get_snapped_center_y(grid_y);
}

void render_init_pieces()
{
    Pawn* pawns = pawn_get_all();
    for (int i = 0; i < game_state->pawn_count; i++)
    {
        render_update_grid_x(&pawns[i]);
        render_update_grid_y(&pawns[i]);
    }
}

void render_init(Game* state)
{
    game_state = state;

    if (TTF_Init() < 0) {
        printf("error: %s\n", SDL_GetError());
    }

    font = TTF_OpenFont("RobotoMono-Regular.ttf", 14);

    if (font == NULL)
    {
        printf("error: %s\n", SDL_GetError());
    }

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

    render_init_pieces();
}

void render_exec(Game* state)
{
    if (window != NULL)
    {
        set_render_colour(1);
        SDL_RenderClear(renderer);
        render_board();
        render_tiles();
        render_pieces();
        render_ui();
        SDL_RenderPresent(renderer);
    }
}

void render_quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
