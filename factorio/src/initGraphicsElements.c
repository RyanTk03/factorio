#include <stdlib.h>
#include <stdio.h>

#include "../inc/mySDLfunc.h"
#include "../inc/const.h"
#include "../inc/AppButton.h"
#include "../inc/initGraphicalsElements.h"


void calculateButtonsPositions(SDL_Point *buttonsPositions)
{
    for(int i = 0; i < TOTAL_BUTTONS - 1; i++)
    {
        if(i == 0 || i == 10)
            buttonsPositions[i].y = SCREEN_HEIGHT - NUMBERBUTTON_H;
        else if((i > 0 && i <= 3) || i == 11)
            buttonsPositions[i].y = SCREEN_HEIGHT - (2*NUMBERBUTTON_H);
        else if((i >= 4 && i <= 6) || i == 12)
            buttonsPositions[i].y = SCREEN_HEIGHT - (3*NUMBERBUTTON_H);
        else if((i >= 7 && i <= 9) || i == 13)
            buttonsPositions[i].y = SCREEN_HEIGHT - (4*NUMBERBUTTON_H);


        if(i == 0 || i == 1 || i == 4 || i == 7)
            buttonsPositions[i].x = 0;
        else if(i == 2 || i == 5 || i == 8)
            buttonsPositions[i].x = NUMBERBUTTON_W;
        else if(i == 3 || i == 6 || i == 9)
            buttonsPositions[i].x = 2*NUMBERBUTTON_W;
        else if(i == 10 || i == 11 || i == 12 || i == 13)
            buttonsPositions[i].x = 3*NUMBERBUTTON_W;
    }
    buttonsPositions[14] = (SDL_Point){513, 17};
}

int initNumberButton(AppButton **button, int len, SDL_Renderer *renderer,
                     TTF_Font *font, SDL_Point *buttonsPositions)
{
    char label[2];

    for(int i = 0; i < len; i++)
    {
        snprintf(label, 1, "%d", i);
        label[1] = '\0';
        button[i] = CreateButton(i != 0 ? NUMBERBUTTON_W : NUMBERBUTTON_W * 3, NUMBERBUTTON_H, label, font);
        if(button[i] == NULL)
            return -1;
        if(CopyButton(button[i], renderer, buttonsPositions[i]) < 0)
        {
            FreeButton(button[i]);
            return -1;
        }
    }

    return 0;
}

int initOperatorButton(AppButton **button, int len, SDL_Renderer *renderer,
                       TTF_Font *font, SDL_Point *buttonsPositions)
{
    char *label = NULL;

    for(int i = 0; i < len; i++)
    {
        switch(i)
        {
            case 0:
                label = ",";
                break;

            case 1:
                label = "+/-";
                break;

            case 2:
                label = "a/b";
                break;

            case 3:
                label = "<--";
                break;
        }
        button[i] = CreateButton(NUMBERBUTTON_W, NUMBERBUTTON_H, label, font);
        if(button[i] == NULL)
            return -1;
        if(CopyButton(button[i], renderer, buttonsPositions[i+10]) < 0)
        {
            FreeButton(button[i]);
            return -1;
        }
    }

    return 0;
}

int initResetButton(AppButton **button, SDL_Renderer *renderer, TTF_Font *font,
                    SDL_Point *buttonsPositions)
{
    *button = CreateButton(60, 56, "Reset", font);

    if(*button == NULL)
        return -1;

    if(CopyButton(*button, renderer, buttonsPositions[14]) < 0)
    {
        FreeButton(*button);
        return -1;
    }

    if(SetButtonColor(*button, renderer, MySDL_COLORWHITE(128), DEFAULT_COLOR) < 0 ||
    SetButtonColor(*button, renderer, MySDL_COLORWHITE(200), HOVER_COLOR) < 0 ||
    SetButtonColor(*button, renderer, MySDL_COLORBLUE(255), ACTIVE_COLOR) < 0)
        return 1;

    return 0;
}
