#include <stdio.h>

#include "../inc/drawFunc.h"
#include "../inc/fraction.h"

void drawBackground(SDL_Renderer *renderer, TTF_Font *font, EditSettings *edit)
{
    /*static SDL_Texture *InputsTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                                            INPUT_WIDTH * 3 + LABEL_XSQUARE.w + LABEL_X.x, INPUT_HEIGHT - 2);*/

    SDL_SetRenderDrawColor(renderer, 63, 72, 204, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 127, 39, 255);
    MySDL_RenderDrawRect(renderer, &(SDL_Rect){7, 17, 506, 56}, 4);

    //Drawing the text boxes where edit the polynome coefficient
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRects(renderer, INPUTS_RECTS, 4);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRects(renderer, INPUTS_RECTS, 4);

    //Draw the graph rect
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &(SDL_Rect){0, 80, SCREEN_WIDTH, 150});
    SDL_SetRenderDrawColor(renderer, 195, 195, 195, 255);
    SDL_RenderDrawRect(renderer, &(SDL_Rect){0, 80, SCREEN_WIDTH, 150});

    //Drawing the text boxes content
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if(edit->modifiedInput != INPUT_NONE)
    {
        char label[MAXNUMBERCHAR + 2];
        SDL_Texture *temp = NULL;
        if(!edit->isFraction[edit->modifiedInput]) //If the number to draw is not a fraction
        {
            int x, y, w, h;
            //Drawing only the numerator
            snprintf(label, MAXNUMBERCHAR + 1, "%+g", Fraction_GetValue(edit->polynome->coefficients[edit->modifiedInput]));
            temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255));
            SDL_QueryTexture(temp, NULL, NULL, &w, &h);
            x = INPUTS_RECTS[edit->modifiedInput].x + INPUT_WIDTH - w - INPUTS_PADDING;
            y = INPUT_Y + (INPUT_HEIGHT / 2) - (h / 2);
            SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
        }
        else //If it is a fraction
        {
            int x1, x2, y1, y2, w1, w2, h1, h2;

            //Drawing first the numerator
            snprintf(label, MAXNUMBERCHAR, "%+d", abs(Fraction_GetNum(edit->polynome->coefficients[edit->modifiedInput])));
            temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255));
            SDL_QueryTexture(temp, NULL, NULL, &w1, &h1);
            x1 = INPUTS_RECTS[edit->modifiedInput].x + INPUT_WIDTH - w1 - INPUTS_PADDING;
            y1 = INPUT_Y;
            SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x1, INPUT_Y, w1, h1});
            SDL_DestroyTexture(temp);

            //Drawing the denominator now
            snprintf(label, MAXNUMBERCHAR, "%d", abs(Fraction_GetDen(edit->polynome->coefficients[edit->modifiedInput])));
            temp = MyTTF_RenderText_Blended(renderer, font, label, MySDL_COLORBLACK(255));
            SDL_QueryTexture(temp, NULL, NULL, &w2, &h2);
            x2 = INPUTS_RECTS[edit->modifiedInput].x + INPUT_WIDTH - w2 - INPUTS_PADDING;
            y2 = INPUTS_RECTS[edit->modifiedInput].y + (INPUT_HEIGHT / 2);
            SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x2, y2, w2, h2});

            //Drawing the fraction's bar
            MySDL_RenderDrawHorizontalLine(renderer, w1 > w2 ? x1 : x2, INPUTS_RECTS[edit->modifiedInput].y + (INPUT_HEIGHT / 2), w1 > w2 ? w1 : w2, 1);
        }
        SDL_DestroyTexture(temp);
    }

    for(int i = 0; i < 3 ; i++)
    {
        SDL_Texture *temp = NULL;
        //Drawing x², x and =. We do it in the same loop(the text boxes content loop) to gain performance
        int x, y, w, h = 19;
        if(i == 0)
        {
            w = LABEL_XSQUARE.x;
            temp = MyTTF_RenderText_Blended(renderer, font, "x²", MySDL_COLORBLACK(255));
        }
        else if(i == 1)
        {
            w = LABEL_X.x;
            temp = MyTTF_RenderText_Blended(renderer, font, "x", MySDL_COLORBLACK(255));
        }
        else if(i == 2)
        {
            w = LABEL_EQUAL.x;
            temp = MyTTF_RenderText_Blended(renderer, font, "=", MySDL_COLORBLACK(255));
        }
        x = INPUTS_RECTS[i].x + INPUT_WIDTH + 5;
        y = INPUT_Y + (INPUT_HEIGHT / 2) - (h / 2);
        SDL_RenderCopy(renderer, temp, NULL, &(SDL_Rect){x, y, w, h});
    }

    if(edit->polynome->r_root1 == NULL && edit->polynome->r_root2 == NULL)
    {

    }

    //drawing the cursor
    static int currentTime = 0, lastTime = 0;
    currentTime = SDL_GetTicks();
    if(currentTime - lastTime >= 1000 && currentTime - lastTime <= 1500)
        SDL_RenderDrawRect(renderer, edit->cursorRect);
    else if(currentTime - lastTime > 1500)
        lastTime = currentTime;

}

void fillCoefficients(char value, EditSettings *edit)
{
    if(value == '\0')
        return;

    char coefficientText[MAXNUMBERCHAR + 1];
    int len = 0;

    if(edit->isFraction[edit->activeInput])
    {
        int num = 0, den = 0;
        if(edit->writtingNumerator[edit->activeInput])
        {
            snprintf(coefficientText, MAXNUMBERCHAR, "%d", Fraction_GetNum(edit->polynome->coefficients[edit->activeInput]));
            len = strlen(coefficientText);
            printf("tst %s\n", coefficientText);

            if(value == '\b' && len > 0)
            {
                coefficientText[len-1] = '\0';

                num = strtol(coefficientText, NULL, 10);
                Fraction_SetNum(edit->polynome->coefficients[edit->activeInput], num);
            }
            else if(value != '\0' && len < MAXNUMBERCHAR)
            {
                coefficientText[len] = value;
                coefficientText[len+1] = '\0';

                num = strtol(coefficientText, NULL, 10);
                Fraction_SetNum(edit->polynome->coefficients[edit->activeInput], num);
            }
        }
        else
        {
            snprintf(coefficientText, MAXNUMBERCHAR, "%d", Fraction_GetDen(edit->polynome->coefficients[edit->activeInput]));
            len = strlen(coefficientText);
            printf("tst %s\n", coefficientText);

            if(value == '\b' && len > 0)
            {
                coefficientText[len-1] = '\0';

                den = strtol(coefficientText, NULL, 10);
                Fraction_SetDen(edit->polynome->coefficients[edit->activeInput], den);
            }
            else if(value != '\0' && len < MAXNUMBERCHAR)
            {
                coefficientText[len] = value;
                coefficientText[len+1] = '\0';

                den = strtol(coefficientText, NULL, 10);
                Fraction_SetDen(edit->polynome->coefficients[edit->activeInput], den);
            }
        }
    }
    else
    {
        double val = Fraction_GetValue(edit->polynome->coefficients[edit->activeInput]);
        printf("%g ", val);
        snprintf(coefficientText, MAXNUMBERCHAR, "%g", val);
        len = val == 0 ? 0 : strlen(coefficientText);
        if(value == '\b' && len > 0)
        {
            coefficientText[len-1] = '\0';

            val = strtod(coefficientText, NULL);
            Fraction_SetValue(edit->polynome->coefficients[edit->activeInput], val);
        }
        else if(value != '\0' && value != '\b' && len < MAXNUMBERCHAR)
        {
            if(value == ',')
                value = '.';

            coefficientText[len] = value;
            coefficientText[len+1] = '\0';

            val = strtod(coefficientText, NULL);
            Fraction_SetValue(edit->polynome->coefficients[edit->activeInput], val);
            printf("%g\n", edit->polynome->coefficients[edit->activeInput]->value);
        }
    }
    edit->modifiedInput = edit->activeInput;
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
