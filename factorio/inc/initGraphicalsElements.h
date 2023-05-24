#ifndef INITGRAPHICALSELEMENTS_H_INCLUDED
#define INITGRAPHICALSELEMENTS_H_INCLUDED

#include <SDL2/SDL.h>

void calculateButtonsPositions(SDL_Point *buttonsPositions);

int initNumberButton(AppButton **button, int len, SDL_Renderer *renderer,
                     TTF_Font *font, SDL_Point *buttonsPositions);

int initOperatorButton(AppButton **button, int len, SDL_Renderer *renderer,
                       TTF_Font *font, SDL_Point *buttonsPositions);

int initResetButton(AppButton **button, SDL_Renderer *renderer, TTF_Font *font,
                    SDL_Point *buttonsPositions);

#endif // INITGRAPHICALSELEMENTS_H_INCLUDED
