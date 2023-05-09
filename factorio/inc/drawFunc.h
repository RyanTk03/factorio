#ifndef DRAWFUNC_H_INCLUDED
#define DRAWFUNC_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "const.h"
#include "mySDLfunc.h"
#include "polynome.h"

typedef enum
{
    INPUT_A = 0, INPUT_B = 1, INPUT_C = 2, INPUT_NONE = 3
} Input;

typedef struct
{
    Input activeInput;
    Input modifiedInput;
    SDL_bool isFraction[3];
    SDL_bool writtingNumerator[3];
    SDL_Rect *cursorRect;
    Polynome *polynome;
} EditSettings;

void drawBackground(SDL_Renderer *renderer, TTF_Font *font, EditSettings *edit);
void fillCoefficients(char value, EditSettings *edit);
void setCursor(SDL_Point *mousePosition, EditSettings *edit);

#endif // DRAWFUNC_H_INCLUDED
