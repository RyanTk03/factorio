#include <math.h>

#include "../inc/fraction.h"

Fraction* CreateFraction()
{
    Fraction *f = malloc(sizeof f);

    f->num = 0;
    f->den = 1;
    f->value = 0;

    return f;
}

void Fraction_SetNum(Fraction *f, double value)
{
    if((int)value != value)
    {
        Fraction f1 = DecimalToFraction(value);
        *f = Fraction_Div(f1, FRACT(f->den));
    }
    else
    {
        f->num = (int)value;
        f->value = (double)f->num / (double)f->den;
    }
}

int Fraction_GetNum(Fraction *f)
{
    return f->num;
}

void Fraction_SetDen(Fraction *f, double value)
{
    if((int)value != value)
    {
        Fraction f1 = DecimalToFraction(value);
        *f = Fraction_Div(FRACT(f->num), f1);
    }
    else
    {
        f->den = (int)value;
        f->value = (double)f->num / (double)f->den;
    }
}

int Fraction_GetDen(Fraction *f)
{
    return f->den;
}

void Fraction_SetValue(Fraction* f, double value)
{
    *f = DecimalToFraction(value);
}

double Fraction_GetValue(Fraction *f)
{
    return f->value;
}

Fraction Fraction_Sum(Fraction f1, Fraction f2)
{
    Fraction result;

    result.num = (f1.num * f2.den) + (f2.num * f1.den);
    result.den = (f1.den * f2.den);
    result.value = (double)result.num / (double)result.den;

    Fraction_Reduce(&result);

    return result;
}

Fraction Fraction_Sub(Fraction f1, Fraction f2)
{
    Fraction result;

    result.num = (f1.num * f2.den) - (f2.num * f1.den);
    result.den = (f1.den * f2.den);
    result.value = (double)result.num / (double)result.den;

    Fraction_Reduce(&result);

    return result;
}

Fraction Fraction_Mul(Fraction f1, Fraction f2)
{
    Fraction result;

    result.num = f1.num * f2.num;
    result.den = f1.den * f2.den;
    result.value = (double)result.num / (double)result.den;

    Fraction_Reduce(&result);

    return result;
}

Fraction Fraction_Div(Fraction f1, Fraction f2)
{
    Fraction result;

    result.num = f1.num * f2.den;
    result.den = f1.den * f2.num;
    result.value = (double)result.num / (double)result.den;

    Fraction_Reduce(&result);

    return result;
}

Fraction Fraction_Pow(Fraction f, int p)
{
    Fraction result;
    result.num = pow(f.num, p);
    result.den = pow(f.den, p);
    result.value = (double)result.num / (double)result.den;

    Fraction_Reduce(&result);

    return result;
}

Fraction Fraction_Sqrt(Fraction f)
{
    Fraction result, f1, f2;

    f1 = DecimalToFraction(sqrt(f.num));
    f2 = DecimalToFraction(sqrt(f.den));

    result = Fraction_Div(f1, f2);

    return result;
}

void Fraction_Reduce(Fraction *f)
{
    if(f->num == 0)
    {
        f->den = 1;
    }
    else
    {
        int d = PGCD(f->num, f->den);

        if(d != 0)
        {
            f->num /= d;
            f->den /= d;
        }
    }
}

Fraction DecimalToFraction(double decimal)
{
    if(decimal == 0)
    {
        return (Fraction){0, 1, 0};
    }
    Fraction result = {0, 1, decimal};
    int d = 1;

    while(decimal - (int)decimal != 0)
    {
        decimal *= 10;
        d *= 10;
    }

    result.num = decimal;
    result.den = d;

    Fraction_Reduce(&result);

    return result;
}

int PGCD(int a, int b)
{
    if(a == 0 || b == 0)
        return 0;

    a = abs(a);
    b = abs(b);

    int r = a > b ? a % b : b % a;

    if(r == 0)
        return a < b ? a : b;
    else
        return PGCD(a < b ? a : b, r);

}
