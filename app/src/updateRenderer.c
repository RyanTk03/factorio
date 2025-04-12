#include <stdio.h>

#include "../inc/updateRenderer.h"

void updateRenderer(SDL_Renderer *renderer, TTF_Font *font)
{
    //Filling window with the background color
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR_COMPOSANT);
    SDL_RenderClear(renderer);

    //delimit the inputs zone by drawing a red rect
    SDL_SetRenderDrawColor(renderer, 255, 127, 39, 255);
    MySDL_RenderDrawRect(renderer, &(SDL_Rect){7, 17, 506, 56}, 4);

    //Drawing x², x and =.
    for(int i = 0; i < 3 ; i++)
    {
        SDL_Texture *temp = NULL;
        int x, y, w, h;
        h = 19;
        if(i == 0)
            temp = MyTTF_RenderText_Blended(renderer, font, "x²", MySDL_COLORBLACK(255), &w, &h), w = 20;
        else if(i == 1)
            temp = MyTTF_RenderText_Blended(renderer, font, "x", MySDL_COLORBLACK(255), &w, &h);
        else if(i == 2)
            temp = MyTTF_RenderText_Blended(renderer, font, "=", MySDL_COLORBLACK(255), &w, &h);
        x = INPUTS_RECTS[i].x + INPUT_WIDTH + 5;
        y = INPUT_Y + (INPUT_HEIGHT / 2) - (h / 2);
        SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
    }
}
