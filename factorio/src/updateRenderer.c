#include <stdio.h>

#include "../inc/updateRenderer.h"

void updateRenderer(SDL_Renderer *renderer)
{
    //Filling window with the background color
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR_COMPOSANT);
    SDL_RenderClear(renderer);

    //delimit the inputs zone by drawing a red rect
    SDL_SetRenderDrawColor(renderer, 255, 127, 39, 255);
    MySDL_RenderDrawRect(renderer, &(SDL_Rect){7, 17, 506, 56}, 4);
}
