#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "../inc/fraction.h"
#include "../inc/const.h"

Fraction* CreateFraction()
{
    Fraction *f = malloc(sizeof (Fraction*));

    f->num = 0;
    f->den = 1;
    f->value = 0;

    return f;
}

void FreeFraction(Fraction *f)
{
    if(f != NULL)
        free(f);
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

SDL_bool Fraction_IsInteger(Fraction *f)
{
    return fmod(Fraction_GetValue(f), 1.0) == 0;
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

SDL_Texture* RenderFraction(SDL_Renderer *renderer, TTF_Font *font, Fraction *fraction, SDL_bool withSign)
{
    SDL_Texture *defaultTarget = SDL_GetRenderTarget(renderer);
    char label[MAXNUMBERCHAR];
    int x1, x2, y1, y2, w1, w2, h1, h2;
    int w;

    //Render first the numerator
    snprintf( label, MAXNUMBERCHAR, withSign ? "%+d" : "%d", withSign ? Fraction_GetNum(fraction) : abs(Fraction_GetNum(fraction)));
    SDL_Texture *temp1 = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w1, &h1);
    SDL_QueryTexture(temp1, NULL, NULL, &w1, &h1);

    //Render the denominator now
    snprintf( label, MAXNUMBERCHAR, "%d", abs( Fraction_GetDen(fraction) ) );
    SDL_Texture *temp2 = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w2, &h2);
    SDL_QueryTexture(temp2, NULL, NULL, &w2, &h2);

    w = (w1 > w2 ? w1 : w2);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, INPUT_HEIGHT - 2);
    SDL_SetRenderTarget(renderer, texture);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    x1 = (w / 2) - (w1 / 2);
    y1 = 0;
    SDL_RenderCopy(renderer, temp1, NULL, &(SDL_Rect){x1, y1, w1, h1});
    SDL_DestroyTexture(temp1);

    x2 = (w / 2) - (w2 / 2);
    y2 = (INPUT_HEIGHT - 2) / 2;
    SDL_RenderCopy(renderer, temp2, NULL, &(SDL_Rect){x2, y2, w2, h2});
    SDL_DestroyTexture(temp2);

    //Drawing the fraction's bar
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    MySDL_RenderDrawHorizontalLine(renderer, w1 > w2 ? x1 : x2, (INPUT_HEIGHT - 2) / 2, w, 1);

    SDL_SetRenderTarget(renderer, defaultTarget);

    return texture;
}
