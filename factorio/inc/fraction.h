#ifndef FRACTION_H_INCLUDED
#define FRACTION_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "mySDLfunc.h"

typedef struct
{
    int num;
    int den;
    double value;
} Fraction;

#define FRACT(n) (Fraction){n, 1, n}

/**
* @brief A function that create a new fraction object.
*
* This function allocate memory for fraction structure and initialize the
* structure by setting numerator to 0, denominator to 0 and value to 1.
*
* @return This function return a pointer to the memory allocates.
*
*/
Fraction* Fraction_Create();

/**
* @brief A function that free a fraction object.
*
* This function free the memory allocate to a fraction object.
*
* @param f(Fraction *) The fraction object to free.
*
*/
void Fraction_Free(Fraction *f);

/**
* @brief A function that give the numerator of a fraction object.
*
* @param f(Fraction *) The fraction object whose we want to get the numerator.
*
* @return The numerator of the fraction given
*
*/
int Fraction_GetNum(Fraction *f);

/**
* @brief A function that give the denominator of a fraction object.
*
* @param f(Fraction *) The fraction object whose we want to get the denominator.
*
* @return The denominator of the fraction given
*
*/
int Fraction_GetDen(Fraction *f);

/**
* @brief A function that set the numerator of a fraction object.
*
* This function changes the numerator of a fraction object and changes also the
* value implicitly.
* In case that the numerator is a floating value, this function will transforms
* it to fraction and will do the compute to changes the numerator and the
* denominator.
*
* @param f(Fraction *) The fraction object whose we want to change the numerator.
* @param value(double) The new numerator of the fraction
*
*/
void Fraction_SetNum(Fraction *f, double value);

/**
* @brief A function that set the denominator of a fraction object.
*
* This function changes the denominator of a fraction object and changes also the
* value implicitly.
* In case that the denominator is a floating value, this function will transforms
* it to fraction and will do the compute to changes the numerator and the
* denominator.
*
* @param f(Fraction *) The fraction object whose we want to change the denominator.
* @param value(double) The new denominator of the fraction
*
*/
void Fraction_SetDen(Fraction *f, double value);

/**
* @brief A function that set the value of a fraction object.
*
* This function changes the value of a fraction object and changes also the
* numerator and the denominator implicitly by convert the value to a fraction.
*
* @param f(Fraction *) The fraction object whose we want to change the value.
* @param value(double) The new value of the fraction
*
*/
void Fraction_SetValue(Fraction* f, double value);

/**
* @brief A function that set the denominator of a fraction object.
*
* This function changes the denominator of a fraction object and changes also the
* value implicitly.
* In case that the denominator is a floating value, this function will transforms
* it to fraction and will do the compute to changes the numerator and the
* denominator.
*
* @param f(Fraction *) The fraction whose we want to get the value.
*
* @return The value of the fraction.
*
*/
double Fraction_GetValue(Fraction *f);

/**
* @brief A function that compute the sum of two fraction object.
*
* @param f1(Fraction *) The first operand.
* @param f2(Fraction *) The second operand.
*
* @return The sum of the fractions.
*
*/
Fraction Fraction_Sum(Fraction *f1, Fraction *f2);

/**
* @brief A function that compute the sum of two fraction object.
*
* @param f1(Fraction *) The first operand.
* @param f2(Fraction *) The second operand.
* @param result(Fraction *) The fraction that contains the result of the compute.
*
*/
void Fraction_SumIn(Fraction *f1, Fraction *f2, Fraction *result);

/**
* @brief A function that compute the substraction of two fraction object.
*
* @param f1(Fraction *) The first operand.
* @param f2(Fraction *) The second operand.
*
* @return The substraction of the fractions.
*
*/
Fraction Fraction_Sub(Fraction *f1, Fraction *f2);

/**
* @brief A function that compute the substraction of two fraction object.
*
* @param f1(Fraction *) The first operand.
* @param f2(Fraction *) The second operand.
* @param result(Fraction *) The fraction that contains the result of the compute.
*
*/
void Fraction_SubIn(Fraction *f1, Fraction *f2, Fraction *result);

/**
* @brief A function that compute the multiplication of two fraction object.
*
* @param f1(Fraction *) The first operand.
* @param f2(Fraction *) The second operand.
*
* @return The multiplication of the fractions.
*
*/
Fraction Fraction_Mul(Fraction *f1, Fraction *f2);

/**
* @brief A function that compute the multiplication of two fraction object.
*
* @param f1(Fraction *) The first operand.
* @param f2(Fraction *) The second operand.
* @param result(Fraction *) The fraction that contains the result of the compute.
*
*/
void Fraction_MulIn(Fraction *f1, Fraction *f2, Fraction *result);

/**
* @brief A function that compute the division of two fraction object.
*
* @param f1(Fraction *) The first operand.
* @param f2(Fraction *) The second operand.
*
* @return The division of the fractions.
*
*/
Fraction Fraction_Div(Fraction *f1, Fraction *f2);

/**
* @brief A function that compute the division of two fraction object.
*
* @param f1(Fraction *) The first operand.
* @param f2(Fraction *) The second operand.
* @param result(Fraction *) The fraction that contains the result of the compute.
*
*/
void Fraction_DivIn(Fraction *f1, Fraction *f2, Fraction *result);

/**
* @brief A function that compute the power of a fraction object to p.
*
* @param f(Fraction *) The fraction object operand.
* @param p(int) The power.
*
* @return The power of the fraction.
*
*/
Fraction Fraction_Pow(Fraction *f, int p);

/**
* @brief A function that compute the power of a fraction object to p.
*
* @param f(Fraction *) The fraction object operand.
* @param p(int) The power.
* @param result(Fraction *) The fraction that contains the result of the compute.
*
*/
void Fraction_PowIn(Fraction *f, int p, Fraction *result);

/**
* @brief A function that compute the square root of a fraction object.
*
* @param f(Fraction *) The fraction.
*
* @return The square root of the fraction.
*
*/
Fraction Fraction_Sqrt(Fraction *f);

/**
* @brief A function that compute the square root of a fraction object.
*
* This function compute the square root like Fraction_Sqrt but not return the
* result. It set the result in the given fraction.
*
* @param f(Fraction *) The fraction whose square root will be compute.
* @param result(Fraction *) The fraction in which set the result.
*
*/
void Fraction_SqrtIn(Fraction *f, Fraction *result);

/**
* @brief A function that that reduce a fraction object.
*
* This function is implicitly uses by others functions once a compute is done
* to update the values of numerator and denominator.
* This function reduce a fraction by calculate first the pgcd of the numerator
* and the denominator before divides each of them with this pgcd.
* The algorithm uses here to computes the pgcd is the algorithm of Euclide.
*
* @param f(Fraction *) The fraction object to reduce.
*
*/
void Fraction_Reduce(Fraction *f);

/**
* @brief A function that convert a decimal value to a fraction.
*
* @param decimal(double) The decimal value to convert.
* @param f(Fraction *) The fraction object to reduce.
*
*/
void DecimalToFraction(double decimal, Fraction *fraction);

/**
* @brief A function that check if a fraction object if contains a floating.
* value or a integer value
*
* @param f(Fraction *) The fraction object to check.
*
*/
SDL_bool Fraction_IsInteger(Fraction *f);

/**
* @brief Change the sign of the fraction to the opposite.
* If it is '+', it becomes '-' and if it is '-', it becomes '+';
*
* @param The fraction to modify.
*
*/
void Fraction_ToggleSign(Fraction *fraction);

/**
* @brief A function that that compute the PGCD of two integer.
*
* The algorithm uses here to computes the pgcd is the algorithm of Euclide.
*
* @param f(Fraction *) The fraction object to reduce.
*
*/
int PGCD(int a, int b);

SDL_Texture* RenderFraction(SDL_Renderer *renderer, TTF_Font *font, Fraction *fraction, SDL_bool withSign, int *w, int *h);

#endif // FRACTION_H_INCLUDED
