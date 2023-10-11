#include "../inc/mySDLfunc.h"
#include <stdio.h>

SDL_Texture* MyTTF_RenderText_Blended(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color fg, int *w, int *h)
{
    SDL_Surface *temp = TTF_RenderText_Blended(font, text, fg);

    if (temp == NULL)
        return NULL;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, temp);

    if (texture == NULL)
        return NULL;

    if(w != NULL)
        *w = temp->w;
    if(h != NULL)
        *h = temp->h;

    SDL_FreeSurface(temp);

    return texture;
}

int MySDL_RenderDrawRect(SDL_Renderer *renderer, const SDL_Rect *rect, int borderWeight)
{
    for(int i = 0; i < borderWeight; i++)
    {
        if(SDL_RenderDrawRect(renderer, &(SDL_Rect){rect->x + i, rect->y + i, rect->w - (2*i), rect->h - (2*i)}) < 0)
            return -1;
    }

    return 0;
}

int MySDL_RenderDrawVerticalLine(SDL_Renderer *renderer, int x, int y, int h, int weight)
{
    for(int i = 0; i < weight; i++)
    {
        if(SDL_RenderDrawLine(renderer, x + i, y, x + i, y + h - 1) < 0)
            return -1;
    }
    return 0;
}

int MySDL_RenderDrawHorizontalLine(SDL_Renderer *renderer, int x, int y, int w, int weight)
{
    for(int i = 0; i < weight; i++)
    {
        if(SDL_RenderDrawLine(renderer, x, y + i, x + w - 1, y + i) < 0)
            return -1;
    }
    return 0;
}

char MySDL_GetNumCharFromKey(SDL_KeyCode code)
{
    char c;
    switch(code)
    {
        case SDLK_0:
        case SDLK_KP_0:
            c = '0';
            break;
        case SDLK_1:
        case SDLK_KP_1:
            c = '1';
            break;
        case SDLK_2:
        case SDLK_KP_2:
            c = '2';
            break;
        case SDLK_3:
        case SDLK_KP_3:
            c = '3';
            break;
        case SDLK_4:
        case SDLK_KP_4:
            c = '4';
            break;
        case SDLK_5:
        case SDLK_KP_5:
            c = '5';
            break;
        case SDLK_6:
        case SDLK_KP_6:
            c = '6';
            break;
        case SDLK_7:
        case SDLK_KP_7:
            c = '7';
            break;
        case SDLK_8:
        case SDLK_KP_8:
            c = '8';
            break;
        case SDLK_9:
        case SDLK_KP_9:
            c = '9';
            break;
        case SDLK_KP_BACKSPACE:
        case SDLK_BACKSPACE:
            c = '\b';
            break;
        case SDLK_COMMA:
        case SDLK_KP_COMMA:
            c = ',';
            break;
        case SDLK_DECIMALSEPARATOR:
        case SDLK_KP_DECIMAL:
            c = '.';
            break;
        default:
            c = '\0';
            break;
    }
    return c;
}
