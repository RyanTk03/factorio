#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "../inc/drawFunc.h"
#include "../inc/fraction.h"
#include "../inc/polynome.h"

void drawBackground(EditSettings *edit)
{
    SDL_SetRenderDrawColor(edit->renderer, BACKGROUND_COLOR_COMPOSANT);
    SDL_RenderClear(edit->renderer);

    SDL_SetRenderDrawColor(edit->renderer, 255, 127, 39, 255);
    MySDL_RenderDrawRect(edit->renderer, &(SDL_Rect){7, 17, 506, 56}, 4);

    //Drawing the text boxes where edit the polynome coefficient
    SDL_SetRenderDrawColor(edit->renderer, 255, 255, 255, 255);
    SDL_RenderFillRects(edit->renderer, INPUTS_RECTS, 4);

    SDL_SetRenderDrawColor(edit->renderer, 100, 100, 100, 255);
    SDL_RenderDrawRects(edit->renderer, INPUTS_RECTS, 4);

    //Draw the graph rect
    SDL_SetRenderDrawColor(edit->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(edit->renderer, &(SDL_Rect){GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT});
    SDL_SetRenderDrawColor(edit->renderer, GRAPH_BORDER_COLOR_COMPOSANT);
    SDL_RenderDrawRect(edit->renderer, &(SDL_Rect){GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT});

    //Draw the graph axes
    SDL_SetRenderDrawColor(edit->renderer, 255, 255, 255, 255);
    MySDL_RenderDrawHorizontalLine(edit->renderer, GRAPH_X, GRAPH_ORIGIN_Y, GRAPH_WIDTH, 1);
    MySDL_RenderDrawVerticalLine(edit->renderer, GRAPH_ORIGIN_X, GRAPH_Y, GRAPH_HEIGHT, 1);

    //Drawing the text boxes content
    SDL_SetRenderDrawColor(edit->renderer, 0, 0, 0, 255);
    for(int i = 0; i < 3 ; i++)
    {
        SDL_Texture *temp = NULL;
        int x, y, w, h;

        if(!edit->isFraction[i]) //If the number to draw is not a fraction
        {
            //Drawing only the numerator
            temp = MyTTF_RenderText_Blended(edit->renderer, edit->font, edit->inputsTexts[i], MySDL_COLORBLACK(255));
            SDL_QueryTexture(temp, NULL, NULL, &w, &h);
            x = INPUTS_RECTS[i].x + INPUT_WIDTH - w - INPUTS_PADDING;
            y = INPUT_Y + (INPUT_HEIGHT / 2) - (h / 2);
            SDL_RenderCopy(edit->renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
            SDL_DestroyTexture(temp);
        }
        else //If it is a fraction
        {
            temp = RenderFraction(edit->renderer, edit->font, edit->polynome->coefficients[i], SDL_TRUE);
            SDL_QueryTexture(temp, NULL, NULL, &w, &h);
            x = INPUTS_RECTS[i].x + INPUT_WIDTH - w - INPUTS_PADDING;
            y = INPUT_Y + (INPUT_HEIGHT / 2) - (h / 2);
            SDL_RenderCopy(edit->renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
            SDL_DestroyTexture(temp);
        }

        //Drawing x², x and =. We do it in the same loop(the text boxes content loop) to gain performance
        h = 19;
        if(i == 0)
        {
            w = LABEL_XSQUARE.x;
            temp = MyTTF_RenderText_Blended(edit->renderer, edit->font, "x²", MySDL_COLORBLACK(255));
        }
        else if(i == 1)
        {
            w = LABEL_X.x;
            temp = MyTTF_RenderText_Blended(edit->renderer, edit->font, "x", MySDL_COLORBLACK(255));
        }
        else if(i == 2)
        {
            w = LABEL_EQUAL.x;
            temp = MyTTF_RenderText_Blended(edit->renderer, edit->font, "=", MySDL_COLORBLACK(255));
        }
        x = INPUTS_RECTS[i].x + INPUT_WIDTH + 5;
        y = INPUT_Y + (INPUT_HEIGHT / 2) - (h / 2);
        SDL_RenderCopy(edit->renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
    }

    drawResult(edit->renderer, edit->font, edit->polynome);

}

void drawResult(SDL_Renderer *renderer, TTF_Font *font, Polynome *polynome)
{
    if(polynome->roots[0] != NULL)
    {
        int w;
        SDL_Texture *rootsTexture = RenderRoots(renderer, font, polynome, &w);
        if(rootsTexture != NULL)
        {
            SDL_RenderCopy(renderer, rootsTexture, &(SDL_Rect){0, 0, w, (INPUT_HEIGHT - 2)},
                       &(SDL_Rect){INPUT4_RECT.x + (INPUT4_RECT.w / 2) - (w / 2), INPUT4_RECT.y + 1, w, (INPUT_HEIGHT - 2)});
        }
    }
    else
    {

    }

}

void flashingCursor(EditSettings *edit)
{
    //drawing the cursor
    static int currentTime = 0, lastTime = 0;
    currentTime = SDL_GetTicks();
    if(currentTime - lastTime >= 1000 && currentTime - lastTime <= 2000)
    {
        SDL_SetRenderDrawColor(edit->renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(edit->renderer, edit->cursorRect);
    }
    else if(currentTime - lastTime > 2000 && currentTime - lastTime <= 3000)
    {
        SDL_SetRenderDrawColor(edit->renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(edit->renderer, edit->cursorRect);
    }
    else
        lastTime = currentTime;
}

void fillCoefficients(char value, EditSettings *edit)
{
    if(value == ',')
        value = '.';

    if(value != '.' && value != '-' && value != '\b' && !isdigit(value))
        return;

    if(value == '.' && strchr(edit->inputsTexts[edit->activeInput], '.') != NULL)
        return;

    int len = 0;

    if(edit->isFraction[edit->activeInput])
    {
        int num = 0, den = 0;
        if(edit->writtingNumerator[edit->activeInput])
        {
            len = strlen(edit->inputsTexts[edit->activeInput]);

            if(value == '\b' && len > 0)
            {
                edit->inputsTexts[edit->activeInput][len-1] = len == 2 ? '0' : '\0';//Because the second character must be '0'

                num = strtol(edit->inputsTexts[edit->activeInput], NULL, 10);
                Fraction_SetNum(edit->polynome->coefficients[edit->activeInput], num);
            }
            else if(value == '-')
            {
                Fraction_SetValue(edit->polynome->coefficients[edit->activeInput], Fraction_GetValue(edit->polynome->coefficients[edit->activeInput]) * -1);
                edit->inputsTexts[edit->activeInput][0] = edit->inputsTexts[edit->activeInput][0] == '+' ? '-' : '+';
            }
            else if(len < MAXNUMBERCHAR + 2)
            {
                edit->inputsTexts[edit->activeInput][len] = value;
                edit->inputsTexts[edit->activeInput][len+1] = '\0';

                num = strtol(edit->inputsTexts[edit->activeInput], NULL, 10);
                Fraction_SetNum(edit->polynome->coefficients[edit->activeInput], num);
            }
        }
        else
        {
            len = strlen(edit->inputsTexts[edit->activeInput]);
            printf("tst %s\n", edit->inputsTexts[edit->activeInput]);

            if(value == '\b' && len > 0)
            {
                edit->inputsTexts[edit->activeInput][len-1] = len == 2 ? '0' : '\0';

                den = strtol(edit->inputsTexts[edit->activeInput], NULL, 10);
                Fraction_SetDen(edit->polynome->coefficients[edit->activeInput], den);
            }
            else if(value == '-')
            {
                Fraction_SetValue(edit->polynome->coefficients[edit->activeInput], Fraction_GetValue(edit->polynome->coefficients[edit->activeInput]) * -1);
                edit->inputsTexts[edit->activeInput][0] = edit->inputsTexts[edit->activeInput][0] == '+' ? '-' : '+';
            }
            else if(len < MAXNUMBERCHAR + 2)
            {
                edit->inputsTexts[edit->activeInput][len] = value;
                edit->inputsTexts[edit->activeInput][len+1] = '\0';

                den = strtol(edit->inputsTexts[edit->activeInput], NULL, 10);
                Fraction_SetDen(edit->polynome->coefficients[edit->activeInput], den);
            }
        }
    }
    else
    {
        double val = Fraction_GetValue(edit->polynome->coefficients[edit->activeInput]);
        len = strlen(edit->inputsTexts[edit->activeInput]);

        if(value == '\b' && len > 0)
        {
            edit->inputsTexts[edit->activeInput][len-1] = len == 2 ? '0' : '\0';

            val = strtod(edit->inputsTexts[edit->activeInput], NULL);
            Fraction_SetValue(edit->polynome->coefficients[edit->activeInput], val);
        }
        else if(value == '-')
        {
            Fraction_SetValue(edit->polynome->coefficients[edit->activeInput], Fraction_GetValue(edit->polynome->coefficients[edit->activeInput]) * -1);
            edit->inputsTexts[edit->activeInput][0] = edit->inputsTexts[edit->activeInput][0] == '+' ? '-' : '+';
        }
        else if(len < MAXNUMBERCHAR + 2)
        {
            edit->inputsTexts[edit->activeInput][val == 0 ? 1 : len] = value;
            edit->inputsTexts[edit->activeInput][len+1] = '\0';

            val = strtod(edit->inputsTexts[edit->activeInput], NULL);
            Fraction_SetValue(edit->polynome->coefficients[edit->activeInput], val);
        }
    }

    factorize(edit->polynome);
}

void setCursor(SDL_Point *mousePosition, EditSettings *edit)
{
    for(int i = 0; i < 3; i++)
    {
        if(SDL_PointInRect(mousePosition, &INPUTS_RECTS[i]))
        {
            edit->cursorRect->x = INPUTS_RECTS[i].x + INPUTS_RECTS[i].w - INPUTS_PADDING;
            if(edit->isFraction[i])
            {
                if(SDL_PointInRect(mousePosition, &(SDL_Rect){INPUTS_RECTS[i].x, INPUTS_RECTS[i].y, INPUTS_RECTS[i].w, INPUTS_RECTS[i].h/2}))
                {
                    edit->cursorRect->y = INPUTS_RECTS[i].y + 2;
                    edit->writtingNumerator[i] = SDL_TRUE;
                }
                else
                {
                    edit->cursorRect->y = INPUT_Y + (INPUT_HEIGHT / 2) + ((INPUT_HEIGHT / 2) /2) - (CURSOR2_HEIGHT / 2);
                    edit->writtingNumerator[i] = SDL_FALSE;
                }
                edit->cursorRect->h = CURSOR2_HEIGHT;
            }
            else
            {
                edit->cursorRect->y  = INPUT_Y + (INPUT_HEIGHT / 2) - (CURSOR1_HEIGHT / 2);
                edit->cursorRect->h = CURSOR1_HEIGHT;
                edit->writtingNumerator[i] = SDL_TRUE;
            }
            edit->activeInput = i;

            break;
        }
    }
}
