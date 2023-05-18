#ifndef DRAWFUNC_H_INCLUDED
#define DRAWFUNC_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "const.h"
#include "mySDLfunc.h"
#include "polynomial.h"

typedef enum
{
    INPUT_A = 0, INPUT_B = 1, INPUT_C = 2, INPUT_NONE = 3
} Input;

typedef struct
{
    Input activeInput;
    SDL_bool isFraction[3];
    SDL_bool writtingNumerator[3];
    SDL_Rect *cursorRect;
    Polynomial *polynomial;
    SDL_Renderer *renderer;
    TTF_Font *font;
    char inputsTexts[3][MAXNUMBERCHAR+2];// +1 for the null character and +1 for the sign(always draw the sign)
    SDL_Texture *polynomialTexture;
} EditSettings;

void updateRenderer(EditSettings *edit);
void updatePolynomial(char value, EditSettings *edit);
void updateCursor(SDL_Point *mousePosition, EditSettings *edit);
void flashingCursor(EditSettings *edit);

#endif // DRAWFUNC_H_INCLUDED
