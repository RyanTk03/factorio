#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../inc/polynomial.h"
#include "../inc/const.h"

Polynomial* Polynomial_Create()
{
    Polynomial *p = malloc(sizeof p);

    if(p == NULL)
        return NULL;

    p->discriminant = CreateFraction();

    for(int i = 0; i < 3 ; i++)
    {
        p->coefficients[i] = CreateFraction();

        if(i < 2)
            p->roots[i] = CreateFraction();
    }

    return p;
}


void Polynomial_ComputeDiscriminant(Polynomial *p, Fraction *f)
{
    *p->discriminant = DecimalToFraction( pow(Fraction_GetValue(p->coefficients[1]), 2) -
                                          (4 * Fraction_GetValue(p->coefficients[0]) * Fraction_GetValue(p->coefficients[2])) );

    if(f != NULL)
        *f = *p->discriminant;
}

void Polynomial_ComputeRoots(Polynomial *p, Fraction *f)
{
    double d = Fraction_GetValue(p->discriminant);

    if(d < 0)
        return;

    for(int i = 0; i < (d == 0 ? 1 : 2); i++)
    {
        if(i == 0)
            *p->roots[i] = DecimalToFraction((- Fraction_GetValue(p->coefficients[1]) -
                                    sqrt(Fraction_GetValue(p->discriminant))) / (2 * Fraction_GetValue(p->coefficients[0])));

        else
            *p->roots[i] = DecimalToFraction((- Fraction_GetValue(p->coefficients[1]) +
                                    sqrt(Fraction_GetValue(p->discriminant))) / (2 * Fraction_GetValue(p->coefficients[0])));
        if(f != NULL)
            f[i] = *p->roots[i];
    }
}

void Polynomial_Factorise(Polynomial *p)
{
    //Only if a != 0
    if(Fraction_GetValue(p->coefficients[0]) != 0)
    {
        Polynomial_ComputeDiscriminant(p, NULL);
        Polynomial_ComputeRoots(p, NULL);
    }
}

int Polynomial_Render(SDL_Renderer *renderer, TTF_Font *font, Polynomial *p)
{
    char label[MAXNUMBERCHAR + 7];

    int x = 0, y = 0, w = 0, h = 0;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Texture *temp;

    int i = 0;
    if(Fraction_GetValue(p->coefficients[0]) == 0 || Fraction_GetValue(p->discriminant) < 0)
    {
        while(p->coefficients[i] != NULL && i < 3)
        {
            if(Fraction_GetValue(p->coefficients[i]) != 0)
            {
                if(Fraction_IsInteger(p->coefficients[i]))
                {
                    snprintf(label, MAXNUMBERCHAR + 7, "%+g%s", Fraction_GetValue(p->coefficients[i]), i == 0 ? "x²" : i == 1 ? "x" : "\0");
                    x = w;
                    temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                    y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);
                }
                else
                {
                    temp = RenderFraction(renderer, font, p->coefficients[i], SDL_TRUE);
                    x += w;
                    SDL_QueryTexture(temp, NULL, NULL, &w, &h);
                    y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);

                    snprintf(label, 3, "%s", i == 0 ? "x²" : i == 1 ? "x" : "\0");
                    x += w;
                    temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                    y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);
                }
            }

            w = x + w; //update the row's width
            i++;
        }
    }
    else
    {
        if(Fraction_GetValue(p->coefficients[0]) != 1)//printing 1 is not necessary
        {
            if(Fraction_IsInteger(p->coefficients[0]))
            {
                snprintf(label, MAXNUMBERCHAR + 7, "%g", Fraction_GetValue(p->coefficients[0]));
                x = w;
                temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                SDL_QueryTexture(temp, NULL, NULL, &w, &h);
                y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                SDL_DestroyTexture(temp);
            }
            else
            {
                temp = RenderFraction(renderer, font, p->coefficients[0], SDL_TRUE);
                x += w;
                SDL_QueryTexture(temp, NULL, NULL, &w, &h);
                y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                SDL_DestroyTexture(temp);
                if(Fraction_GetValue(p->discriminant) == 0)
                {
                    x += w;
                    temp = MyTTF_RenderText_Blended(renderer, font, "x", MySDL_COLORBLACK(255), &w, &h);
                    y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);
                }
            }
            w = x + w; //update the row's width
        }

        while(p->roots[i] != NULL && i < 2)
        {
            if(Fraction_GetValue(p->roots[i]) != 0)
            {
                if(Fraction_IsInteger(p->roots[i]))
                {
                    snprintf(label, MAXNUMBERCHAR + 7, "( x %+g )%c", -Fraction_GetValue(p->roots[i]), Fraction_GetValue(p->discriminant) == 0 ? '²' : '\0');
                    x = w;
                    temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                    SDL_QueryTexture(temp, NULL, NULL, &w, &h);
                    y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);
                }
                else
                {
                    snprintf(label, MAXNUMBERCHAR + 5, "( x %c ", Fraction_GetValue(p->roots[i]) < 0 ? '+' : '-');
                    x = w;
                    temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                    SDL_QueryTexture(temp, NULL, NULL, &w, &h);
                    y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);

                    temp = RenderFraction(renderer, font, p->roots[i], SDL_FALSE);
                    x += w;
                    SDL_QueryTexture(temp, NULL, NULL, &w, &h);
                    y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);

                    x += w;
                    snprintf(label, MAXNUMBERCHAR + 7, " )%c", Fraction_GetValue(p->discriminant) == 0 ? '²' : '\0');
                    temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                    SDL_QueryTexture(temp, NULL, NULL, &w, &h);
                    y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);
                }
            }
            else
            {
                x = w;
                snprintf(label, MAXNUMBERCHAR + 7, "(x)%c", Fraction_GetValue(p->discriminant) == 0 ? '²' : '\0');
                temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                SDL_QueryTexture(temp, NULL, NULL, &w, &h);
                y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
                SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                SDL_DestroyTexture(temp);
            }

            w = x + w; //update the row's width
            i++;

            if(Fraction_GetValue(p->discriminant) == 0)
                break;
        }
    }

    return w;

}

int Polynomial_DrawGraph(Polynomial *p, SDL_Renderer *renderer, SDL_Point origin)
{
    SDL_FPoint point;
    for (int i = -320; i < 320; i++)
    {
        point.x = origin.x + i;
        point.y = origin.y - ((GRAPH_HEIGHT / 2) * SDL_pow(((Fraction_GetValue(p->coefficients[0]) * pow(i, 2)) +
                   (Fraction_GetValue(p->coefficients[1]) * i) + Fraction_GetValue(p->coefficients[2])) / (GRAPH_HEIGHT / 2), 0.5));
        if(SDL_RenderDrawPointF(renderer, point.x, point.y) < 0)
            return -1;
    }

    return 0;
}

void Polynomial_Destroy(Polynomial *p)
{
    if(p->discriminant != NULL)
        free(p->discriminant);

    for(int i = 0; i < 3 ; i++)
    {
        if(p->coefficients[i] != NULL)
            free(p->coefficients[i]);

        if(i < 2)
            if(p->roots[i])
                free(p->roots[i]);
    }
}
