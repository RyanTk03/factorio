#include <stdlib.h>

#include "../inc/const.h"
#include "../inc/AppButton.h"
#include "../inc/initGraphicalsElements.h"


SDL_Point buttonsPositions[TOTAL_BUTTONS];

void calculateButtonsPositions()
{
    for(int i = 0; i < TOTAL_BUTTONS; i++)
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
}

int initNumberButton(AppButton *button[], int len, SDL_Renderer *renderer, TTF_Font *font)
{
    char label[2];

    for(int i = 0; i < len; i++)
    {
        sprintf(label, "%d", i);
        label[1] = '\0';
        button[i] = CreateButton(i != 0 ? NUMBERBUTTON_W : NUMBERBUTTON_W * 3, NUMBERBUTTON_H, label, font);
        if(button[i] == NULL)
            return -1;
        CopyButton(button[i], renderer, buttonsPositions[i]);
    }

    return 0;
}

int initOperatorButton(AppButton *button[], int len, SDL_Renderer *renderer, TTF_Font *font)
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
        CopyButton(button[i], renderer, buttonsPositions[i+10]);
    }

    return 0;
}

int initOthersButton(AppButton *button[], int len, SDL_Renderer *renderer, TTF_Font *font)
{
    for(int i = 0; i < len; i++)
    {
        button[i] = CreateButton(100, 40, i == 0 ? "Factorise" : "Reset", font);
        if(button[i] == NULL)
            return -1;
        CopyButton(button[i], renderer, (SDL_Point){513, 25});
    }

    return 0;
}
