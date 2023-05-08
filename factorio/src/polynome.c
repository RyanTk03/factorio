#include <stdlib.h>
#include <math.h>

#include "../inc/polynome.h"

void discriminant(Polynome *p, Fraction *f)
{
    p->r_discriminant = CreateFraction();
    *p->r_discriminant = DecimalToFraction(pow(Fraction_GetValue(p->coefficients[1]), 2) -
                                          (4 * Fraction_GetValue(p->coefficients[0]) * Fraction_GetValue(p->coefficients[2])));

    if(f != NULL)
        *f = *p->r_discriminant;
}

void roots(Polynome *p, Fraction *f)
{
    double d = Fraction_GetValue(p->r_discriminant);

    if(d < 0)
        return;

    p->r_root1 = CreateFraction();
    *p->r_root1 = DecimalToFraction((- Fraction_GetValue(p->coefficients[1]) - sqrt(Fraction_GetValue(p->r_discriminant))) /
                                        (2 * Fraction_GetValue(p->coefficients[0])));
    if(f != NULL)
        f[0] = *p->r_root1;

    if(d == 0)
        return;

    p->r_root2 = CreateFraction();
    *p->r_root2 = DecimalToFraction((- Fraction_GetValue(p->coefficients[1]) + sqrt(Fraction_GetValue(p->r_discriminant))) /
                                        (2 * Fraction_GetValue(p->coefficients[0])));

    if(f != NULL)
        f[1] = *p->r_root2;

}

void factorize(Polynome *p)
{
    discriminant(p, NULL);
    roots(p, NULL);
}
