#ifndef POLYNOME_H_INCLUDED
#define POLYNOME_H_INCLUDED

#include "fraction.h"

typedef struct
{
    Fraction* coefficients[3];
    Fraction *discriminant;
    Fraction *roots[2];
    SDL_Point points[640];
} Polynomial;

Polynomial* Polynomial_Create();
void Polynomial_ComputeDiscriminant(Polynomial *p, Fraction *f);
void Polynomial_ComputeRoots(Polynomial *p, Fraction *f);
void Polynomial_Factorise(Polynomial *p);
int Polynomial_Render(SDL_Renderer *renderer, TTF_Font *font, Polynomial *p);
int Polynomial_DrawGraph(Polynomial *p, SDL_Renderer *renderer, SDL_Point origin);
void Polynomial_Destroy(Polynomial *p);

#endif // POLYNOME_H_INCLUDED
