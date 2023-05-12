#ifndef POLYNOME_H_INCLUDED
#define POLYNOME_H_INCLUDED

#include "fraction.h"

typedef struct
{
    Fraction* coefficients[3];
    Fraction *r_discriminant;
    Fraction *roots[2];
} Polynome;

void discriminant(Polynome *p, Fraction *f);
void roots(Polynome *p, Fraction *f);
void factorize(Polynome *p);
SDL_Texture* RenderRoots(SDL_Renderer *renderer, TTF_Font *font, Polynome *p, int *width);

#endif // POLYNOME_H_INCLUDED
