#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <SDL2/SDL.h>
#include "polynomial.h"

//The maximum character in an input without sign
#define INPUT_MAX_CHARACTER 18

#define INPUT_FRACTIONMOD 'f'
#define INPUT_KLEFT 'l'
#define INPUT_KRIGHT 'r'
#define INPUT_KUP 'u'
#define INPUT_KDOWN 'd'
#define INPUT_KBACK '\b'
#define INPUT_KTAB '\t'
#define INPUT_KNULL '\0'


///This structure represent an input cursor.
typedef struct
{
    int x;
    int y;
    int h;
} InputCursor;

/**
* A simple sdl input object to input this program or write data like text box
* control.
*
*/
typedef struct
{
    ///Define if the input has the focus
    SDL_bool hasFocus;

    ///Define if the input is display as a fraction or not.
    SDL_bool isFraction;

    ///In case that the input type is a fraction, define if writing a numerator.
    SDL_bool writingNumerator;

    ///The rectangle where the input is displaying.
    SDL_Rect rect;

    ///The cursor of the input.
    InputCursor *cursor;

    ///The current position of the cursor.
    Uint8 currentPosition;

    ///The data to update when the input is updated.
    double *dataLinked;

    ///The texts to print : text[0]="numerator"; text[1]="denominator".
    char text[2][INPUT_MAX_CHARACTER + 2];
} Input;

typedef enum
{
    INPUT_MOUSEBUTTONDOWN, INPUT_KEYDOWN, INPUT_NOEVENT
} InputEventType;

typedef struct
{
    char keyValue;
    SDL_Point mousePosition;
    InputEventType type;
} InputEvent;


void Input_Init(SDL_Point charSize);


/**
* @brief Create a new input type.
*
* @param rect(SDL_Rect) The rectangle where draw the input
* @param cursor(InputCursor) The cursor used for each input
* @param datalinked(double*) A pointer to the data corresponding to the input
*        data. So, Once the input is updated, the data(variable) is also
*        updated.
*
* @return A pointer to the input create.
*
*/
Input* Input_Create(SDL_Renderer *renderer, SDL_Rect *rect, InputCursor *cursor, double *dataLinked, SDL_bool hasFocus);


/**
* @brief Free the memory allocate for an input create previously.
*
* @param The input to free.
*
*/
void Input_Destroy(Input *input);


/**
* @brief Update the input content.
*
* It look at the event manager to check the event happened and updates the input
* according to this event.
*
* @param input(Input*) The input to update.
* @param event(event*) The event manager.
*
*/
void Input_Update(Input *input, SDL_Renderer *renderer, TTF_Font *font, InputEvent *event);

#endif // INPUT_H_INCLUDED
