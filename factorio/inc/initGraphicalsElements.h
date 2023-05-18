#ifndef INITGRAPHICALSELEMENTS_H_INCLUDED
#define INITGRAPHICALSELEMENTS_H_INCLUDED

int initNumberButton(AppButton **button, int len, SDL_Renderer *renderer, TTF_Font *font);
int initOperatorButton(AppButton **button, int len, SDL_Renderer *renderer, TTF_Font *font);
int initResetButton(AppButton **button, SDL_Renderer *renderer, TTF_Font *font);
void calculateButtonsPositions();

#endif // INITGRAPHICALSELEMENTS_H_INCLUDED
