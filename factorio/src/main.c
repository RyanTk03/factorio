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
    AppButton *resetButton;
    //AppButton *resetButton;

    Polynome polynome = {{CreateFraction(), CreateFraction(), CreateFraction()}, NULL, {NULL, NULL}}; //The polynome to factorize
    EditSettings edit = {.activeInput = INPUT_A, .isFraction = {SDL_FALSE, SDL_FALSE, SDL_FALSE},
                         .writtingNumerator = {SDL_TRUE, SDL_TRUE, SDL_TRUE}, .cursorRect = &cursorRect, .polynome = &polynome,
                         .renderer = appRenderer, .inputsTexts = {{"+0"}, {"+0"}, {"+0"}}, .font = font};

    SDL_SetRenderDrawColor(appRenderer, BACKGROUND_COLOR_COMPOSANT);
    SDL_RenderClear(appRenderer);

    calculateButtonsPositions();
    if(initNumberButton(numberButton, 10, appRenderer, font) < 0)
        printf("Erreur : %s \n", SDL_GetError());

    if(initOperatorButton(operatorButton, 4, appRenderer, font) < 0)
        printf("Erreur : %s \n", SDL_GetError());

    //if(initOthersButton((AppButton*[]){resetButton, resetButton}, 1, appRenderer, font) < 0)
    //    printf("Erreur : %s \n", SDL_GetError());
    resetButton = CreateButton(60, 56, "Reset", font);
    CopyButton(resetButton, appRenderer, (SDL_Point){513, 17});
    SetButtonColor(resetButton, appRenderer, MySDL_COLORWHITE(128), DEFAULT_COLOR);
    SetButtonColor(resetButton, appRenderer, MySDL_COLORWHITE(200), HOVER_COLOR);
    SetButtonColor(resetButton, appRenderer, MySDL_COLORBLUE(255), ACTIVE_COLOR);

    SDL_RenderPresent(appRenderer);

    SDL_StartTextInput();

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while(!quit)
    {
        SDL_WaitEventTimeout(&event, 1000);
        switch(event.type)
        {
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
            case SDL_TEXTINPUT:
                printf("-%c\n", event.text.text[0]);
                fillCoefficients(event.text.text[0], &edit);
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_KP_BACKSPACE)
                    fillCoefficients('\b', &edit);
                break;
            case SDL_MOUSEBUTTONDOWN:
                setCursor(&(SDL_Point){event.button.x, event.button.y}, &edit);

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
                if(IsSelected(resetButton))
                {
                    free(polynome.r_discriminant);
                    for(int i = 0; i < 3; i++)
                    {
                        Fraction_SetValue(polynome.coefficients[i], 0);
                        edit.inputsTexts[i][0] = '+'; edit.inputsTexts[i][1] = '0';
                        edit.isFraction[i] = SDL_FALSE;
                        edit.writtingNumerator[i] = SDL_TRUE;
                        if(i < 2)
                        {
                            free(polynome.roots[i]);
                            polynome.roots[i] = NULL;
                        }
                    }
                }
                break;
        }

        drawBackground(&edit);

        for(int i = 0; i < 10; i++)
            UpdateButton(numberButton[i], appRenderer, &event);
        for(int i = 0; i < 4; i++)
            UpdateButton(operatorButton[i], appRenderer, &event);
        UpdateButton(resetButton, appRenderer, &event);

        flashingCursor(&edit);
        SDL_RenderPresent(appRenderer);
    }

    SDL_StopTextInput();

    //If the program execute even here all done well so,
    exit_statut = EXIT_SUCCESS;

    for(int i = 0; i < 10; i++)
        FreeButton(numberButton[i]);

    for(int i = 0; i < 4; i++)
        FreeButton(operatorButton[i]);

    FreeButton(resetButton);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(appRenderer);
    SDL_DestroyWindow(appWindow);


    TTF_Quit();
    SDL_Quit();
    return exit_statut;
}
