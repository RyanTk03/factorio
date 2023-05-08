#ifndef MYSDLFUNC_H_INCLUDED
#define MYSDLFUNC_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MySDL_COLORWHITE(a) (SDL_Color){255, 255, 255, a}
#define MySDL_COLORBLACK(a) (SDL_Color){0, 0, 0, a}
#define MySDL_COLORRED(a) (SDL_Color){255, 0, 0, a}
#define MySDL_COLORBLUE(a) (SDL_Color){0, 0, 255, a}
#define MySDL_COLORGREEN(a) (SDL_Color){0, 255, 0, a}
#define MySDL_COLORYELLOW(a) (SDL_Color){255, 255, 0, a}

SDL_Texture* MyTTF_RenderText_Blended(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color fg);
int MySDL_RenderDrawRect(SDL_Renderer *renderer, const SDL_Rect *rect, int borderWeight);
int MySDL_RenderDrawVerticalLine(SDL_Renderer *renderer, int x, int y, int h, int weight);
int MySDL_RenderDrawHorizontalLine(SDL_Renderer *renderer, int x, int y, int w, int weight);
char MySDL_GetNumCharFromKey(SDL_KeyCode code);

#endif // MYSDLFUNC_H_INCLUDED
