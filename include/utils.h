#ifndef UTILS_H
#define UTILS_H

int clamp_uint32(int d, int min, int max);
int SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius);
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
#endif