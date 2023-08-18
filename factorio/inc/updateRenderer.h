#ifndef DRAWFUNC_H_INCLUDED
#define DRAWFUNC_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "const.h"
#include "mySDLfunc.h"

/**
* @brief A function that draw the window contains.
*
* This function draw and paints the rectangles and colors the different
* part of the render.
*
* @param edit(EditState *) A pointer to the default state of the program.
* @param font(TTF_Font *) A pointer to the font to use.
*
*/
void updateRenderer(SDL_Renderer *renderer, TTF_Font *font);

#endif // DRAWFUNC_H_INCLUDED
