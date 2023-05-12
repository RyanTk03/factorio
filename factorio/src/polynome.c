#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../inc/polynome.h"
#include "../inc/const.h"

void discriminant(Polynome *p, Fraction *f)
{
    if(p->r_discriminant == NULL)
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

    for(int i = 0; i < 2; i++)
    {
        if(p->roots[i] == NULL)
            p->roots[i] = CreateFraction();
        if(i == 0)
            *p->roots[i] = DecimalToFraction((- Fraction_GetValue(p->coefficients[1]) -
                                    sqrt(Fraction_GetValue(p->r_discriminant))) / (2 * Fraction_GetValue(p->coefficients[0])));

        else
            *p->roots[i] = DecimalToFraction((- Fraction_GetValue(p->coefficients[1]) +
                                    sqrt(Fraction_GetValue(p->r_discriminant))) / (2 * Fraction_GetValue(p->coefficients[0])));
        if(f != NULL)
            f[i] = *p->roots[i];
    }
}

void factorize(Polynome *p)
{
    discriminant(p, NULL);
    roots(p, NULL);
}

SDL_Texture* RenderRoots(SDL_Renderer *renderer, TTF_Font *font, Polynome *p, int *width)
{
    if(p->roots == NULL || (p->roots[0] == NULL && p->roots[1] == NULL))
        return NULL;

    char label[MAXNUMBERCHAR + 7];

    int x = 0, y = 0, w = 0, h = 0;

    SDL_Texture *defaultTarget = SDL_GetRenderTarget(renderer);
    SDL_Texture *rootsTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                        INPUT_WIDTH * 3, INPUT_HEIGHT - 2);
    SDL_SetRenderTarget(renderer, rootsTexture);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Texture *temp;

    if(fmod(Fraction_GetValue(p->coefficients[0]), 1.0) == 0)
    {
        snprintf(label, MAXNUMBERCHAR + 7, "%g", Fraction_GetValue(p->coefficients[0]));
        temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255));
        x = w;
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
    }
    w = x + w; //update the row's width

    int i = 0;
    while(p->roots[i] != NULL && i < 2)
    {
        if(fmod(Fraction_GetValue(p->roots[i]), 1.0) == 0)
        {
            snprintf(label, MAXNUMBERCHAR + 7, "( x %+g )", Fraction_GetValue(p->roots[i]));
            temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255));
            x = w;
            SDL_QueryTexture(temp, NULL, NULL, &w, &h);
            y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
            SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
            SDL_DestroyTexture(temp);
        }
        else
        {
            snprintf(label, MAXNUMBERCHAR + 5, "( x %c ", Fraction_GetValue(p->roots[i]) < 0 ? '-' : '+');
            temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255));
            x = w;
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

            temp = MyTTF_RenderText_Blended(renderer, font, " )", MySDL_COLORBLACK(255));
            x += w;
            SDL_QueryTexture(temp, NULL, NULL, &w, &h);
            y = ((INPUT_HEIGHT - 2) / 2) - (h / 2);
            SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
            SDL_DestroyTexture(temp);
        }
        w = x + w; //update the row's width
        i++;
    }
    SDL_SetRenderTarget(renderer, defaultTarget);

    *width = w;
    return rootsTexture;

}
