#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../inc/polynomial.h"
#include "../inc/const.h"

Polynomial* Polynomial_Create(SDL_Renderer *renderer, SDL_Rect *resultRect, SDL_Rect *graphRect, int charWidth)
{
    Polynomial *p = malloc(sizeof(Polynomial));

    if(p != NULL)
    {
        p->result.rect = *resultRect;

        p->graph.rect = *graphRect;
        p->graph.viewStart.x = -(graphRect->w / 2);
        p->graph.viewStart.y = graphRect->h / 2;

        p->graph.origin.x = graphRect->x + graphRect->w / 2;
        p->graph.origin.y = graphRect->y + graphRect->h / 2;

        p->result.viewStart.x = 0;
        p->result.viewStart.y = 0;

        p->result.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, ((charWidth * INPUT_MAX_CHARACTER) * 3) + (charWidth * 3), resultRect->h);
        p->result.totalSize.y = resultRect->h;

        for(int i = 0; i < 3; i++)
        {
            p->coefficients[i] = 0;
            if(i < 2)
                p->roots[i] = 0;
        }
    }

    return p;
}


void Polynomial_ComputeDiscriminant(Polynomial *p, float *d)
{
    p->discriminant = pow(p->coefficients[1], 2) - (4 * p->coefficients[0] * p->coefficients[2]);

    if(d != NULL)
        *d = p->discriminant;
}

void Polynomial_ComputeRoots(Polynomial *p, float *r, int n)
{
    if(p->discriminant < 0)
        return;

    p->roots[0] = (- p->coefficients[1] - sqrt(p->discriminant)) / (2 * p->coefficients[0]);
    if(r != NULL && n >= 1)
        r[0] = p->roots[0];

    if(p->discriminant > 0)
    {
        p->roots[1] = (- p->coefficients[1] + sqrt(p->discriminant)) / (2 * p->coefficients[0]);
        if(r != NULL && n > 1)
            r[1] = p->roots[1];
    }
}

void Polynomial_Factorise(Polynomial *p)
{
    //Only if a != 0
    if(p->coefficients[0] != 0)
    {
        Polynomial_ComputeDiscriminant(p, NULL);
        Polynomial_ComputeRoots(p, NULL, 0);

    }
}

int Polynomial_DrawCoefficient(Polynomial *p, SDL_Renderer *renderer, TTF_Font *font, int c, int posX, SDL_bool withX)
{
    if(p->coefficients[c] != 0)
    {
        SDL_Texture *temp = NULL;
        char label[INPUT_MAX_CHARACTER + 4];
        int y, w, h;

        if(fmod(p->coefficients[c], 1.0) == 0)//If the coefficient is an integer
        {
            snprintf(label, INPUT_MAX_CHARACTER + 4, "%+g%s", p->coefficients[c], withX && c == 0 ? "x²" : withX && c == 1 ? "x" : "\0");
            temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
            y = (p->result.rect.h / 2) - (h / 2);
            SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){posX, y, w, h});
            SDL_DestroyTexture(temp);
        }
        else //It will be render as a fraction
        {
            //first, render the coefficient as a fraction.
            Fraction f;
            DecimalToFraction(p->coefficients[c], &f);
            temp = RenderFraction(renderer, font, &f, SDL_TRUE, &w, &h);
            y = (p->result.rect.h / 2) - (h / 2);
            SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){posX, y, w, h});
            SDL_DestroyTexture(temp);

            //now render the literal if it should be drawed
            if(withX)
            {
                posX += w;
                snprintf(label, 3, "%s", c == 0 ? "x²" : c == 1 ? "x" : "\0");
                temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                y = (p->result.rect.h / 2) - (h / 2);
                SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){posX, y, w, h});
                SDL_DestroyTexture(temp);
            }
        }
        return posX + w;
    }

    return posX;
}

void Polynomial_RenderResult(Polynomial *p, SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_Texture *temp;
    char label[INPUT_MAX_CHARACTER + 9];// + 9 because of the other character like (, ), x, x² and the space.
    int x = 0, y = 0, w = 0, h = 0;

    SDL_Texture *target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, p->result.texture);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);


    int i = 0;

    // If the polynomial is not factorisable, or is not of degree 2
    if(p->coefficients[0] == 0 || p->discriminant < 0)
    {// Render the polynomial in primary form(render the coefficient and x or x²)
        for(i = 0; i < 3; i++)
        {
            x = Polynomial_DrawCoefficient(p, renderer, font, i, x, SDL_TRUE);
            w = x;
        }
    }
    else
    {// Render the factorised form.
        // First, write the coefficient[0] (a).
        if(p->coefficients[0] != 1)//printing 1 is not necessary
        {
            x = Polynomial_DrawCoefficient(p, renderer, font, 0, x, SDL_FALSE);
            w = x;
        }

        while(i < 2)
        {
            if(p->roots[i] != 0)
            {
                if(fmod(p->roots[i], 1.0) == 0)
                {
                    snprintf(label, INPUT_MAX_CHARACTER + 9, "( x %+g )%c", -p->roots[i], p->discriminant == 0 ? '²' : '\0');
                    x = w;
                    temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                    y = (p->result.rect.h / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);
                }
                else
                {
                    snprintf(label, MAXNUMBERCHAR + 5, "( x %c ", p->roots[i] < 0 ? '+' : '-');
                    x = w;
                    temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                    y = (p->result.rect.h / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);

                    Fraction f;
                    DecimalToFraction(p->roots[i], &f);
                    x += w;
                    temp = RenderFraction(renderer, font, &f, SDL_FALSE, &w, &h);
                    y = (p->result.rect.h / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);

                    x += w;
                    snprintf(label, 4, " )%c", p->discriminant == 0 ? '²' : '\0');
                    temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                    y = (p->result.rect.h / 2) - (h / 2);
                    SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                    SDL_DestroyTexture(temp);
                }
            }
            else
            {
                x = w;
                snprintf(label, MAXNUMBERCHAR + 7, "(x)%c", p->discriminant == 0 ? '²' : '\0');
                temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255), &w, &h);
                y = (p->result.rect.h / 2) - (h / 2);
                SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
                SDL_DestroyTexture(temp);
            }

            w = x + w; //update the row's width
            i++;

            if(p->discriminant == 0)
                break;
        }
    }

    SDL_SetRenderTarget(renderer, target);

    p->result.totalSize.x = w;
}

void Polynomial_UpdateResult(Polynomial *p, SDL_Renderer *renderer, TTF_Font *font, SDL_Event *event, InputEvent *ievent, SDL_bool wasModified)
{
    if(wasModified)
    {
        Polynomial_Factorise(p);
    Polynomial_RenderResult(p, renderer, font);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer, &p->result.rect);

    if(event->type == SDL_MOUSEBUTTONDOWN && SDL_PointInRect(&(SDL_Point){event->button.x, event->button.y}, &p->result.rect)
       && p->result.totalSize.x > p->result.rect.w)
    {
        p->result.hasFocus = SDL_TRUE;
        SDL_bool quit = SDL_FALSE;
        ievent->hasFocus = NULL;

        while(!quit)
        {
            SDL_WaitEvent(event);
            switch(event->type)
            {
                case SDL_MOUSEBUTTONUP:
                    quit = SDL_TRUE;
                    break;

                case SDL_MOUSEMOTION:
                    p->result.viewStart.x -= event->motion.xrel;
                    if(p->result.viewStart.x < 0)
                        p->result.viewStart.x = 0;
                    else if(p->result.viewStart.x > p->result.totalSize.x - p->result.rect.w)
                        p->result.viewStart.x = p->result.totalSize.x - p->result.rect.w;
                    break;
            }
            SDL_RenderCopy(renderer, p->result.texture,
                           &(SDL_Rect){p->result.viewStart.x, 0, p->result.rect.w, p->result.rect.h},
                            &(SDL_Rect){p->result.rect.x, p->result.rect.y, p->result.rect.w, p->result.rect.h});

            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderDrawRect(renderer, &p->result.rect);
            SDL_RenderPresent(renderer);
        }
        return;
    }
    else if(event->type == SDL_MOUSEBUTTONDOWN &&
        !SDL_PointInRect(&(SDL_Point){event->button.x, event->button.y}, &p->result.rect) && ievent->hasFocus != NULL)
    {
        p->result.hasFocus = SDL_FALSE;
    }
    else if(p->result.hasFocus && event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.sym == SDLK_LEFT)
            p->result.viewStart.x -= 10;
        else if(event->key.keysym.sym == SDLK_RIGHT)
            p->result.viewStart.x += 10;

        if(p->result.viewStart.x < 0)
            p->result.viewStart.x = 0;
        else if(p->result.viewStart.x > p->result.totalSize.x - p->result.rect.w)
            p->result.viewStart.x = p->result.totalSize.x - p->result.rect.w;
    }

    if(p->result.totalSize.x < p->result.rect.w)
    {
        SDL_RenderCopy(renderer, p->result.texture,
                       &(SDL_Rect){0, 0, p->result.totalSize.x, p->result.rect.h},
                       &(SDL_Rect){p->result.rect.x + (p->result.rect.w / 2) - (p->result.totalSize.x / 2),
                                  p->result.rect.y, p->result.totalSize.x, p->result.totalSize.y});
    }
    else
    {
        SDL_RenderCopy(renderer, p->result.texture,
                        &(SDL_Rect){p->result.viewStart.x, 0, p->result.rect.w, p->result.rect.h},
                        &(SDL_Rect){p->result.rect.x, p->result.rect.y, p->result.rect.w, p->result.rect.h});
    }

    if(p->result.hasFocus)
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderDrawRect(renderer, &p->result.rect);
}

void Polynomial_DrawGraphBox(Polynomial *p, SDL_Renderer *renderer)
{
    //Draw the graph rectangle
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &p->graph.rect);
    SDL_SetRenderDrawColor(renderer, 195, 195, 195, 255);
    SDL_RenderDrawRect(renderer, &p->graph.rect);
    //Draw the graph axes
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if(p->graph.rect.y + p->graph.viewStart.y > p->graph.rect.y &&
       p->graph.rect.y + p->graph.viewStart.y < p->graph.rect.y + p->graph.rect.h)
        MySDL_RenderDrawHorizontalLine(renderer, p->graph.rect.x, p->graph.rect.y + p->graph.viewStart.y,
                                        p->graph.rect.w, 1);

    if(p->graph.rect.x - p->graph.viewStart.x > p->graph.rect.x &&
       p->graph.rect.x - p->graph.viewStart.x < p->graph.rect.x + p->graph.rect.w)
        MySDL_RenderDrawVerticalLine(renderer, p->graph.rect.x - p->graph.viewStart.x, p->graph.rect.y,
                                        p->graph.rect.h, 1);

    SDL_SetRenderTarget(renderer, NULL);
}

int Polynomial_DrawGraphPoints(Polynomial *p, SDL_Renderer *renderer)
{
    SDL_Point lastPoint = {p->graph.rect.x - 100, p->graph.rect.y - 100};
    SDL_Point currentPoint;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for(int i = 0; i < p->graph.rect.w; i++)
    {
        currentPoint.x = p->graph.origin.x - p->graph.rect.x - i;
        for(int j = 0; j < p->graph.rect.h; j++)
        {
            currentPoint.y =  p->graph.origin.y - p->graph.rect.y - j;

            if(currentPoint.y == (p->coefficients[0] * pow(currentPoint.x, 2)) +
               (p->coefficients[1] * currentPoint.x) +
               p->coefficients[2])
            {
                int x = p->graph.origin.x - currentPoint.x;
                int y = p->graph.origin.y - currentPoint.y;

                if(SDL_PointInRect(&lastPoint, &p->graph.rect))
                    if(SDL_RenderDrawLine(renderer, lastPoint.x, lastPoint.y, x, y) < 0)
                        return -1;
                else
                    if(SDL_RenderDrawPoint(renderer, x, y) < 0)
                        return -1;

                lastPoint.x = x;
                lastPoint.y = y;
            }
        }
    }

    return 0;
}

int Polynomial_UpdateGraph(Polynomial *p, SDL_Renderer *renderer, SDL_Event *event, InputEvent *ievent)
{
    if(event->type == SDL_MOUSEBUTTONDOWN && SDL_PointInRect(&(SDL_Point){event->motion.x, event->motion.y}, &p->graph.rect))
    {
        SDL_bool quit = SDL_FALSE;
        int8_t stat = 0;

        while(!quit)
        {
            SDL_WaitEvent(event);
            switch(event->type)
            {
                case SDL_MOUSEBUTTONUP:
                    quit = SDL_TRUE;
                    break;

                case SDL_MOUSEMOTION:
                    p->graph.origin.x += event->motion.xrel;
                    p->graph.origin.y += event->motion.yrel;
                    p->graph.viewStart.x -= event->motion.xrel;
                    p->graph.viewStart.y += event->motion.yrel;

                    break;
            }

            Polynomial_DrawGraphBox(p, renderer);
            stat = Polynomial_DrawGraphPoints(p,renderer);

            SDL_RenderPresent(renderer);
        }
        return stat;
    }
    else
    {
        Polynomial_DrawGraphBox(p, renderer);
        return Polynomial_DrawGraphPoints(p,renderer);
    }
}

void Polynomial_Destroy(Polynomial *p)
{
    if(p != NULL)
    {
        SDL_DestroyTexture(p->result.texture);
        free(p);
    }
}
