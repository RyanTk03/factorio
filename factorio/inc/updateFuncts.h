#ifndef DRAWFUNC_H_INCLUDED
#define DRAWFUNC_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "const.h"
#include "mySDLfunc.h"
#include "polynomial.h"

//An enumeration for each coefficient's input
typedef enum
{
    INPUT_A = 0, INPUT_B = 1, INPUT_C = 2, INPUT_NONE = 3
} Input;

//A structure which represent the state of the program
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
} EditState;

/**
* @brief A function that initialize the program state.
*
* @param edit(EditState *) A pointer to the default state of the program.
* @param renderer(SDL_Renderer *) The renderer of the program.
* @param font(TTF_Font *) The font use in the program.
* @param p(Polynomial *) The polynomial which is editing in the program.
* @param polynomialTexture(SDL_Texture *) The texture where the polynomial
*        is draw before to be drew in the render(the window)
* @param cursor(SDL_Rect *) The rectangle which represent the cursor
*
*/
void initEditState(EditState *edit, SDL_Renderer *renderer, TTF_Font *font,
                   Polynomial *p, SDL_Texture *polynomialTexture,
                   SDL_Rect *cursor);

/**
* @brief A function that draw the window contains.
*
* This function draw and paints the rectangles and colors the different
* part of the render.
*
* @param edit(EditState *) A pointer to the default state of the program.
*
*/
void updateRenderer(EditState *edit);

/**
* @brief A function that update the polynomial.
*
* This function updates the coefficients by concatenates the last value
* with the new value and converts the result to a real value.
*
* This function is called when a numeric key of keyboard or a button on
* the window was pressed.
*
* @param value(char) The value to add in.
* @param edit(EditState *) A pointer to the default state of the program.
*
*/
void updatePolynomial(char value, EditState *edit);

/**
* @brief A function that update the cursor.
*
* This function updates the cursor position by checking where the mouse
* were clicked to see which input was selected. If the no input was clicked,
* the cursor position don't changes.
*
* @param mousePosition(SDL_Point *) A pointer to the mouse position.
* @param edit(EditState *) A pointer to the default state of the program.
*
*/
void updateCursor(SDL_Point *mousePosition, EditState *edit);

/**
* @brief A function that fake a the cursor blinking.
*
* This function updates the cursor color with white or black at a constant
* interval to fake a cursor flashing
*
* @param edit(EditState *) A pointer to the default state of the program.
*
*/
void flashingCursor(EditState *edit);

#endif // DRAWFUNC_H_INCLUDED
