#ifndef POLYNOME_H_INCLUDED
#define POLYNOME_H_INCLUDED

#include "fraction.h"

typedef struct
{
    Fraction* coefficients[3];
    Fraction *r_discriminant;
    Fraction *r_root1;
    Fraction *r_root2;
} Polynome;

void discriminant(Polynome *p, Fraction *f);
void roots(Polynome *p, Fraction *f);
void factorize(Polynome *p);

#endif // POLYNOME_H_INCLUDED
