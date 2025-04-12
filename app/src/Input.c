#include "../inc/Input.h"
#include "../inc/const.h"
#include "../inc/fraction.h"

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

    int indexEnd = strlen(str) - 1;

    int i;
    for(i = index; i < indexEnd; i++)
        str[i] = str[i+1];

    str[i] = '\0';
}

static void Input_InsertChar(char* str, int index, char ch)
{
    if(str == NULL)
        return;

    int indexEnd = strlen(str);

    //First, move the others character to the right
    for(int i = indexEnd; i >= index; i--)
        str[i+1] = str[i];

    //Now set the character(ch) to the position wanted
    str[index] = ch;
}

void Input_Init(SDL_Point charSize)
{
    Input_charSize = charSize;
}

Input* Input_Create(SDL_Renderer *renderer, SDL_Rect *rect, InputCursor *cursor, float *dataLinked, SDL_bool hasFocus)
{
    Input *input = malloc(sizeof(Input));

    if(input != NULL)
    {
        input->hasFocus = hasFocus;
        input->dataWasModified = SDL_FALSE;

        input->cursor = cursor;

        input->currentNumPosition = 1;
        input->currentDenPosition = 1;

        input->rect = (SDL_Rect){rect->x, rect->y, rect->w, rect->h};

        input->writingNumerator = SDL_TRUE;
        input->isFraction = SDL_FALSE;

        for(int i = 0; i < 2; i++)
        {
            input->text[i][0] = i == 0 ? '+' : '1';
            input->text[i][1] = i == 0 ? '0' : '\0';
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
    if(input == NULL)
        return;

    //We verify if the key entered is valid(neither alphabetic key nor decimal virgule if a decimal virgule is already set).
    if(event->keyValue == INPUT_KNULL ||
       (event->keyValue == '.' && SDL_strchr(input->writingNumerator ? input->text[0] :
                                         input->text[1], '.') != NULL))
        return;

    //Check if the user change input sign
    if(event->keyValue == '-')
    {
        *input->dataLinked *= -1;
        input->text[0][0] = input->text[0][0] == '-' ? '+' : '-';
    }
    //Check if the user change fraction mode
    else if(event->keyValue == INPUT_FRACTIONMOD)
    {
        input->isFraction = !input->isFraction;
        if(input->isFraction)
        {
            input->cursor->h = CURSOR_FRACTION_HEIGHT;
            input->cursor->y = input->rect.y + ((input->rect.h / 2) / 2) - (input->cursor->h / 2);
        }
        else
        {
            input->cursor->h = CURSOR_HEIGHT;
            input->cursor->y = input->rect.y + (input->rect.h / 2) - (input->cursor->h / 2);
        }
    }
    //Check if user change the cursor position to left by enter left key
    else if(event->keyValue == INPUT_KLEFT || event->keyValue == INPUT_KRIGHT)
    {
        if(event->keyValue == INPUT_KLEFT)
        {
            if(input->writingNumerator && input->currentNumPosition > 1)
            {
                input->currentNumPosition--;
                input->cursor->x -= Input_charSize.x;

            }
            else if(!input->writingNumerator && input->currentDenPosition > 0)
            {
                input->currentDenPosition--;
                input->cursor->x -= Input_charSize.x;
            }
        }
        else
        {
            if(input->writingNumerator && input->currentNumPosition < strlen(input->text[0]))
            {
                input->currentNumPosition++;
                input->cursor->x += Input_charSize.x;
            }
            else if(!input->writingNumerator && input->currentDenPosition < strlen(input->text[1]))
            {
                input->currentDenPosition++;
                input->cursor->x += Input_charSize.x;
            }
        }
    }
    //Check if user got to edit numerator or denominator by pressing up or down key
    else if(input->isFraction && (event->keyValue == INPUT_KUP || event->keyValue == INPUT_KDOWN))
    {
        input->writingNumerator = !input->writingNumerator;

        if(input->writingNumerator)
        {
            input->cursor->y = input->rect.y + ((input->rect.h / 2) / 2) - (input->cursor->h / 2);
            input->cursor->x = CURSOR_STARTING_X(input->rect) - (Input_charSize.x * (strlen(input->text[0]) - input->currentNumPosition));
        }
        else
        {
            input->cursor->y = input->rect.y + ((input->rect.h / 4) * 3) - (input->cursor->h / 2);
            input->cursor->x = CURSOR_STARTING_X(input->rect) - (Input_charSize.x * (strlen(input->text[1]) - input->currentDenPosition));
        }
    }
    else if(event->keyValue == INPUT_KBACK || SDL_isdigit(event->keyValue))
    {
        SDL_bool wasModified = SDL_FALSE;

        if(event->keyValue == INPUT_KBACK)
        {
            if(input->writingNumerator && input->currentNumPosition > 1)
            {
                input->currentNumPosition--;
                Input_RemoveChar(input->text[0], input->currentNumPosition);
                wasModified = SDL_TRUE;
            }
            else if(!input->writingNumerator && input->currentDenPosition > 0)
            {
                input->currentDenPosition--;
                Input_RemoveChar(input->text[1], input->currentDenPosition);
                wasModified = SDL_TRUE;
            }
        }
        else
        {
            if(input->writingNumerator && strlen(input->text[0]) < INPUT_MAX_CHARACTER + 1)
            {
                if(strlen(input->text[0]) == 2 && input->text[0][1] == '0')
                    input->text[0][1] = '\0';
                Input_InsertChar(input->text[0], input->currentNumPosition, event->keyValue);
                input->currentNumPosition++;
                wasModified = SDL_TRUE;
            }

            else if(!input->writingNumerator && strlen(input->text[1]) < INPUT_MAX_CHARACTER)
            {
                Input_InsertChar(input->text[1], input->currentDenPosition, event->keyValue);
                input->currentDenPosition++;
                wasModified = SDL_TRUE;
            }
        }
        input->dataWasModified = wasModified;

        if(!wasModified)
            return;

        if(input->isFraction)
        {
            float num, den;

            num = SDL_atof(&input->text[0][1]);

            if(SDL_strlen(input->text[1]) == 0)
                den = 1;
            else
                den = SDL_atof(input->text[1]);

            *input->dataLinked = num / den;
        }
        else
        {
            *input->dataLinked = SDL_atof(&input->text[0][1]);
        }
        printf("%f\n", *input->dataLinked);
    }
}

void Input_ManageMouseDownEvent(Input *input, InputEvent *event)
{
    if(input == NULL)
        return;

    if(event->hasFocus == input)
    {
        input->hasFocus = SDL_TRUE;
        input->cursor->x = input->rect.x + input->rect.w - INPUT_PADDING;
    }
    else if(event->hasFocus != NULL)
        input->hasFocus = SDL_FALSE;

}

void Input_Update(Input *input, SDL_Renderer *renderer, TTF_Font *font, InputEvent *event)
{
    if(input == NULL)
        return;

    input->dataWasModified = SDL_FALSE;

    SDL_Texture *content = NULL;
    int contentWidth, contentHeight;

    //Drawing the input rect
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &input->rect);

    //Check if the input has focus
    if(input->hasFocus)
    {
        //Manage key down
        if(event->type == INPUT_KEYDOWN)
        {
            Input_ManageInputEvent(input, event);
        }

        //Set the focused border
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderDrawRect(renderer, &input->rect);
    }
    //Manage the mouse button down event
    if(event->type == INPUT_MOUSEBUTTONDOWN)
    {
        Input_ManageMouseDownEvent(input, event);
    }

    //Now rendering the input content
    //If the input is on fraction mod, get a fraction render
    if(input->isFraction)
    {
        Fraction f;
        DecimalToFraction(*input->dataLinked, &f);
        content = Input_RenderFraction(input, renderer, font, &contentWidth, &contentHeight);
    }
    //Else get a decimal render
    else
    {
        content = MyTTF_RenderText_Blended(renderer, font, input->text[0], (SDL_Color){0, 0, 0, 255}, &contentWidth, &contentHeight);
    }

    int w = contentWidth > (input->rect.w + (INPUT_PADDING * 2)) ? input->rect.w - (INPUT_PADDING * 2) : contentWidth;
    int x = contentWidth > (input->rect.w + (INPUT_PADDING * 2)) ?
        input->rect.x + INPUT_PADDING :
        (input->rect.x + input->rect.w) - INPUT_PADDING - w;
    SDL_RenderCopy(renderer, content,
                   &(SDL_Rect){contentWidth - w, 0, w, contentHeight},
                   &(SDL_Rect){x, input->rect.y + (input->rect.h / 2) - (contentHeight / 2), w, contentHeight});

    //Now render the cursor
    Input_UpdateCursor(input, renderer);
}

void Input_UpdateCursor(Input *input, SDL_Renderer *renderer)
{
    static int currentTime = 0, lastTime = 0;
    currentTime = SDL_GetTicks();

    if(currentTime - lastTime > 2000)
        lastTime = currentTime;

    if(currentTime - lastTime >= 800 && currentTime - lastTime <= 1600)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &(SDL_Rect){input->cursor->x, input->cursor->y, 1, input->cursor->h});
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &(SDL_Rect){input->cursor->x, input->cursor->y, 1, input->cursor->h});
    }
}

SDL_Texture* Input_RenderFraction(Input *input, SDL_Renderer *renderer, TTF_Font *font, int *contentWidth, int *contentHeight)
{
    SDL_Texture *defaultTarget = SDL_GetRenderTarget(renderer);
    int x1, x2, y1, y2, w1, w2, h1, h2;
    int width;

    //Render first the numerator
    SDL_Texture *temp1 = MyTTF_RenderText_Blended(renderer, font, input->text[0], MySDL_COLORBLACK(255), &w1, &h1);
    SDL_QueryTexture(temp1, NULL, NULL, &w1, &h1);

    //Render the denominator now
    SDL_Texture *temp2 = MyTTF_RenderText_Blended(renderer, font, SDL_strlen(input->text[1]) == 0 ? " " : input->text[1], MySDL_COLORBLACK(255), &w2, &h2);
    SDL_QueryTexture(temp2, NULL, NULL, &w2, &h2);

    width = (w1 > w2 ? w1 : w2);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, h1 + h2);
    SDL_SetRenderTarget(renderer, texture);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    x1 = width - w1;
    y1 = 0;
    SDL_RenderCopy(renderer, temp1, NULL, &(SDL_Rect){x1, y1, w1, h1});
    SDL_DestroyTexture(temp1);

    x2 = width - w2;
    y2 = (INPUT_HEIGHT - 2) / 2;
    SDL_RenderCopy(renderer, temp2, NULL, &(SDL_Rect){x2, y2, w2, h2});
    SDL_DestroyTexture(temp2);

    //Drawing the fraction's bar
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    MySDL_RenderDrawHorizontalLine(renderer, w1 > w2 ? x1 : x2, y1 + h1, width , 1);

    SDL_SetRenderTarget(renderer, defaultTarget);

    if(contentWidth != NULL)
        *contentWidth = width;
    if(contentHeight != NULL)
        *contentHeight = h1 + h2;

    return texture;
}

void Input_Reset(Input *input)
{
    input->text[0][0] = '+';
    input->text[0][1] = '0';
    input->text[1][0] = '1';

    for(int i = 2; i < strlen(input->text[0]); i++)
        input->text[0][i] = '\0';
    for(int i = 2; i < strlen(input->text[1]); i++)
        input->text[1][i] = '\0';

    *input->dataLinked = 0;
    input->dataWasModified = SDL_TRUE;

    input->isFraction = SDL_FALSE;

    input->currentNumPosition = 1;
    input->currentDenPosition = 0;
}
