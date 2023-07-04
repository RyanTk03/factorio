#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../inc/Input.h"
#include "../inc/const.h"

/*
An array which contains width the character that the user can set size.
In our case, only the number(0-9) and the sign(+/-). this array is fill once
the program is lunch in the main()
*/
static SDL_Point Input_charSize = {0};

static void Input_RemoveChar(char* str, int index)
{
    if(str == NULL)
        return;

    int length = strlen(str);
    memmove(&str[index], &str[index + 1], length - index);
}

static void Input_InsertChar(char* str, int index, char ch)
{
    if(str == NULL)
        return;

    int length = strlen(str);
    memmove(&str[index + 1], &str[index], length - index + 1);
    str[index] = ch;
}

void Input_Init(SDL_Point charSize)
{
    Input_charSize = charSize;
}

Input* Input_Create(SDL_Renderer *renderer, SDL_Rect *rect, InputCursor *cursor, double *dataLinked, SDL_bool hasFocus)
{
    Input *input = malloc(sizeof(Input));

    if(input != NULL)
    {
        input->hasFocus = hasFocus;

        input->cursor = cursor;

        input->rect = (SDL_Rect){rect->x, rect->y, rect->w, rect->h};

        input->writingNumerator = SDL_TRUE;
        input->isFraction = SDL_FALSE;

        for(int i = 0; i < 2; i++)
        {
            input->text[i][0] = '+';
            input->text[i][1] = i == 0 ? '0' : '1';
            input->text[i][2] = '\0';
        }

        input->dataLinked = dataLinked;
    }

    return input;
}

void Input_Destroy(Input *input)
{
    if(input != NULL)
        free(input);
}

void Input_ManageInputEvent(Input *input, InputEvent *event)
{
    printf("%d-",event->keyValue);
    if(event->keyValue == '\0' ||
       (event->keyValue == '.' && strchr(input->writingNumerator ? input->text[0] :
                                         input->text[1], '.') != NULL))
        return;

    if(event->keyValue == '-')
    {
        *input->dataLinked *= -1;
        input->text[0][0] = input->text[0][0] == '-' ? '+' : '-';
    }
    else if(event->keyValue == INPUT_FRACTIONMOD)
    {
        input->isFraction = !input->isFraction;
    }
    else if(event->keyValue == INPUT_KLEFT && input->currentPosition > 1)
    {
        input->currentPosition--;
    }
    else if(event->keyValue == INPUT_KRIGHT && input->currentPosition > 1)
    {
         input->currentPosition++;
    }
    else if(event->keyValue == INPUT_KUP && input->currentPosition > 1)
    {
        input->currentPosition--;
    }
    else if(event->keyValue == INPUT_KDOWN && input->currentPosition > 1)
    {
        input->currentPosition++;
    }
    else
    {
        if(event->keyValue == INPUT_KBACK)
        {
            Input_RemoveChar(input->writingNumerator && input->currentPosition > 1 ?
                             input->text[0] : !input->writingNumerator ?
                             input->text[1] : NULL, input->currentPosition);

        }
        else if(event->keyValue != INPUT_KNULL && input->writingNumerator ?
                strlen(input->text[0]) < INPUT_MAX_CHARACTER + 1: strlen(input->text[1]) <
                INPUT_MAX_CHARACTER)//For the numerator, we do +1 because of sign but
                                    //we don't want to print the denominator's sign
        {

            Input_InsertChar(input->writingNumerator ? input->text[0] :
                             input->text[1], input->currentPosition, event->keyValue);

        }

        if(input->isFraction)
        {
            double num, den;

            sscanf(input->text[0], "%g", &num);
            sscanf(input->text[1], "%g", &den);

            *input->dataLinked = num / den;
        }
        else
        {
            sscanf(input->text[0], "%g", input->dataLinked);
        }
    }
}

void Input_ManageMouseDownEvent(Input *input, InputEvent *event)
{
    if(SDL_PointInRect(&event->mousePosition, &input->rect))
    {
        input->hasFocus = SDL_TRUE;
        int x = 0;
        event->mousePosition.x -= input->rect.x;
        event->mousePosition.x = input->rect.w - event->mousePosition.x - INPUTS_PADDING;
        x = floor((float)input->rect.w / event->mousePosition.x);
        input->cursor->x = (input->rect.x + input->rect.w - INPUTS_PADDING) - (x * Input_charSize.x);
        if(input->isFraction)
        {
            input->cursor->h = CURSOR2_HEIGHT;
            if(SDL_PointInRect(&event->mousePosition, &(SDL_Rect){input->rect.x,
                               input->rect.y, input->rect.w, input->rect.h/2}))
            {
                input->cursor->y = input->rect.y + 1;
                input->writingNumerator = SDL_TRUE;
            }
            else
            {
                input->cursor->y = input->rect.y + (input->rect.h / 2) + ((input->rect.h / 2) /2) - (input->cursor->h / 2);
                input->writingNumerator = SDL_FALSE;
            }
        }
        else
        {
            input->cursor->h = CURSOR1_HEIGHT;
            input->cursor->y = input->rect.y + (input->rect.h / 2) - (input->cursor->h / 2);
            input->writingNumerator = SDL_TRUE;
        }
    }
    else
        input->hasFocus = SDL_FALSE;

}

void Input_Update(Input *input, SDL_Renderer *renderer, TTF_Font *font, InputEvent *event)
{
    if(input->hasFocus)
    {
        SDL_Texture *content = NULL;
        int contentWidth, contentHeight;

        if(event->type == INPUT_KEYDOWN)
        {
            Input_ManageInputEvent(input, event);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &input->rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderDrawRect(renderer, &input->rect);


    }


    if(input->isFraction)
    {
        Fraction f;
        DecimalToFraction(*input->dataLinked, &f);
        content = RenderFraction(renderer, font, &f, SDL_TRUE, &contentWidth, &contentHeight);
    }
    else
    {
        content = MyTTF_RenderText_Blended(renderer, font, input->text[0], (SDL_Color){0, 0, 0, 255}, &contentWidth, &contentHeight);
    }

    if(contentWidth > input->rect.w)
    {
        SDL_RenderCopy(renderer, content,
                       &(SDL_Rect){0, 0, Input_charSize.x, 19},
                       &(SDL_Rect){input->rect.x, input->rect.y,
                                    Input_charSize.x, 19});

        int w = input->rect.w - Input_charSize.x;

        SDL_RenderCopy(renderer, content,
                       &(SDL_Rect){contentWidth - input->rect.w + Input_charSize.x, 0,
                                   w, contentHeight},
                       &(SDL_Rect){input->rect.x + Input_charSize.x,
                                   input->rect.y + (input->rect.h/2) - (contentHeight/2),
                                   w, contentHeight});
    }
    else
    {
        SDL_RenderCopy(renderer, content,
                       &(SDL_Rect){0, 0, contentWidth, contentHeight},
                       &(SDL_Rect){input->rect.x + input->rect.w - contentWidth,
                                   input->rect.y + (input->rect.h/2) - (contentHeight/2),
                                   contentWidth, contentHeight});
    }

    if(event->type == INPUT_MOUSEBUTTONDOWN)
    {
        Input_ManageMouseDownEvent(input, event);
    }
    static int currentTime = 0, lastTime = 0;
    currentTime = SDL_GetTicks();
    if(currentTime - lastTime >= 1000 && currentTime - lastTime <= 2000)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &(SDL_Rect){input->cursor->x, input->cursor->y, 1, input->cursor->h});
    }
    else if(currentTime - lastTime > 2000 && currentTime - lastTime <= 3000)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &(SDL_Rect){input->cursor->x, input->cursor->y, 1, input->cursor->h});
    }
    else
        lastTime = currentTime;
}
