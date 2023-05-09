#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../inc/AppButton.h"
#include "../inc/initGraphicalsElements.h"
#include "../inc/const.h"
#include "../inc/mySDLfunc.h"
#include "../inc/polynome.h"
#include "../inc/drawFunc.h"


int main(int argc, char * argv[])
{
    int exit_statut = EXIT_FAILURE;

    if(SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0)
    {
        fprintf(stderr, "main(int, char**): Erreur lors de l'initialisation de la SDL(%s).", SDL_GetError());
        return exit_statut;
    }

    SDL_Window *appWindow = SDL_CreateWindow("Factorio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);;
    SDL_Renderer *appRenderer = SDL_CreateRenderer(appWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_Rect cursorRect = {INPUT1_RECT.x + INPUT_WIDTH - INPUTS_PADDING,
                            INPUT_Y + (INPUT_HEIGHT / 2) - (CURSOR1_HEIGHT / 2), 1, CURSOR1_HEIGHT};
    TTF_Font *font = TTF_OpenFont("tahoma.ttf", 15);

    AppButton *numberButton[10];
    AppButton *operatorButton[4];
    AppButton *factoriserButton;
    AppButton *resetButton;
    SDL_Point buttonsPositions[TOTAL_BUTTONS];

    Polynome polynome = {{CreateFraction(), CreateFraction(), CreateFraction()}, NULL, NULL, NULL}; //The polynome to factorize
    EditSettings edit = {.activeInput = INPUT_A, .modifiedInput = INPUT_NONE, .isFraction = {SDL_FALSE, SDL_FALSE, SDL_FALSE},
                         .writtingNumerator = {SDL_TRUE, SDL_TRUE, SDL_TRUE}, .cursorRect = &cursorRect, .polynome = &polynome};

    SDL_SetRenderDrawColor(appRenderer, BACKGROUND_COLOR_COMPOSANT);
    SDL_RenderClear(appRenderer);

    calculateButtonsPositions();
    if(initNumberButton(numberButton, 10, appRenderer, font) < 0)
        printf("Erreur : %s \n", SDL_GetError());

    if(initOperatorButton(operatorButton, 4, appRenderer, font) < 0)
        printf("Erreur : %s \n", SDL_GetError());

    //if(initOthersButton((AppButton*[]){factoriserButton, resetButton}, 1, appRenderer, font) < 0)
    //    printf("Erreur : %s \n", SDL_GetError());
    factoriserButton = CreateButton(100, 40, "Factoriser", font);
    CopyButton(factoriserButton, appRenderer, (SDL_Point){513, 33});
    SetButtonColor(factoriserButton, appRenderer, MySDL_COLORYELLOW(255), DEFAULT_COLOR);
    SetButtonColor(factoriserButton, appRenderer, MySDL_COLORYELLOW(200), HOVER_COLOR);
    SetButtonColor(factoriserButton, appRenderer, BACKGROUND_COLOR, ACTIVE_COLOR);

    SDL_RenderPresent(appRenderer);

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while(!quit)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
            case SDL_KEYDOWN:
                fillCoefficients(MySDL_GetNumCharFromKey(event.key.keysym.sym), &edit);
                break;
            case SDL_MOUSEBUTTONDOWN:
                setCursor(&(SDL_Point){event.button.x, event.button.y}, &edit);
                break;
        }

        for(int i = 0; i < 10; i++)
            if(IsSelected(numberButton[i]))
                fillCoefficients(numberButton[i]->label[0], &edit);

        for(int i = 0; i < 6; i++)
        {
            if(IsSelected(operatorButton[i]))
            {
                switch(i)
                {
                    case 0:
                        break;

                    case 1:
                        Fraction_SetValue(edit.polynome->coefficients[edit.activeInput], Fraction_GetValue(edit.polynome->coefficients[edit.activeInput]) * -1);
                        break;

                    case 2:
                        edit.isFraction[edit.activeInput] = !edit.isFraction[edit.activeInput];
                        setCursor(&(SDL_Point){INPUTS_RECTS[edit.activeInput].x + 50, event.button.y}, &edit);
                        break;

                    case 3:
                        fillCoefficients('\b', &edit);
                        break;
                }
            }
        }

        if(IsSelected(factoriserButton))
            factorize(edit.polynome);


        drawBackground(appRenderer, font, &edit);

        for(int i = 0; i < 10; i++)
            UpdateButton(numberButton[i], appRenderer, &event);
        for(int i = 0; i < 4; i++)
            UpdateButton(operatorButton[i], appRenderer, &event);
        UpdateButton(factoriserButton, appRenderer, &event);
        SDL_RenderPresent(appRenderer);
    }

    //If the program execute even here all done well so,
    exit_statut = EXIT_SUCCESS;
    SDL_DestroyRenderer(appRenderer);
    SDL_DestroyWindow(appWindow);
    TTF_CloseFont(font);

    for(int i = 0; i < 10; i++)
        FreeButton(numberButton[i]);

    for(int i = 0; i < 4; i++)
        FreeButton(operatorButton[i]);

    FreeButton(factoriserButton);

    SDL_Quit();
    TTF_Quit();
    return exit_statut;
}
