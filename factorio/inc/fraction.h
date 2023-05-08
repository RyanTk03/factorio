#ifndef FRACTION_H_INCLUDED
#define FRACTION_H_INCLUDED

typedef struct
{
    int num;
    int den;
    double value;
} Fraction;

#define FRACT(n) (Fraction){n, 1, n}

Fraction* CreateFraction();
int Fraction_GetNum(Fraction *f);
int Fraction_GetDen(Fraction *f);
void Fraction_SetNum(Fraction *f, double value);
void Fraction_SetDen(Fraction *f, double value);
void Fraction_SetValue(Fraction* f, double value);
double Fraction_GetValue(Fraction *f);
Fraction Fraction_Sum(Fraction f1, Fraction f2);
Fraction Fraction_Sub(Fraction f1, Fraction f2);
Fraction Fraction_Mul(Fraction f1, Fraction f2);
Fraction Fraction_Div(Fraction f1, Fraction f2);
Fraction Fraction_Pow(Fraction f, int p);
Fraction Fraction_Sqrt(Fraction f);
void Fraction_Reduce(Fraction *f);
Fraction DecimalToFraction(double decimal);
int PGCD(int a, int b);

#endif // FRACTION_H_INCLUDED
