#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../inc/AppButton.h"
#include "../inc/initGraphicalsElements.h"
#include "../inc/const.h"
#include "../inc/mySDLfunc.h"
#include "../inc/polynomial.h"
#include "../inc/updateFuncts.h"


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

    //The button of the app
    AppButton *numberButton[10] = {};
    AppButton *operatorButton[4] = {};
    AppButton *resetButton = NULL;

    //The polynomial to factorize
    Polynomial *polynomial = Polynomial_Create();

    SDL_Texture *polynomialTexture = SDL_CreateTexture(appRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                        INPUT_WIDTH * 3, INPUT_HEIGHT - 2);

    //Contains all the edits settings(the input which is modified, the cursor rect, if an input contains a fraction)
    EditSettings edit = {.activeInput = INPUT_A, .isFraction = {SDL_FALSE, SDL_FALSE, SDL_FALSE},
                         .writtingNumerator = {SDL_TRUE, SDL_TRUE, SDL_TRUE}, .cursorRect = &cursorRect, .polynomial = polynomial,
                         .renderer = appRenderer, .inputsTexts = {{"+0"}, {"+0"}, {"+0"}}, .font = font, .polynomialTexture = polynomialTexture};

    SDL_SetRenderDrawColor(appRenderer, BACKGROUND_COLOR_COMPOSANT);
    SDL_RenderClear(appRenderer);

    //Fill the static global array in "initGraphicsElements.c". Is is an array of SDL_Point for the position of the button
    calculateButtonsPositions();

    if(initNumberButton(numberButton, 10, appRenderer, font) < 0 ||
       initOperatorButton(operatorButton, 4, appRenderer, font) < 0 ||
       initResetButton(&resetButton, appRenderer, font) < 0)
    {
        printf("Erreur : %s \n", SDL_GetError());
        goto EXIT_TAG;
    }

    SDL_RenderPresent(appRenderer);

    SDL_StartTextInput();

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while(!quit)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;

            case SDL_TEXTINPUT:
                //Update the coefficients value
                updatePolynomial(event.text.text[0], &edit);
                break;

            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_KP_BACKSPACE)
                    updatePolynomial('\b', &edit);
                else if(event.key.keysym.sym == SDLK_LEFT)
                {
                    if(edit.activeInput - 1 != INPUT_NONE)
                        edit.activeInput -= 1;
                    else
                        edit.activeInput = INPUT_C;
                }
                else if(event.key.keysym.sym == SDLK_RIGHT)
                {
                    if(edit.activeInput + 1 != INPUT_NONE)
                    {
                        edit.activeInput += 1;
                        updateCursor(NULL, &edit);
                    }
                    else
                    {
                        edit.activeInput = INPUT_A;
                        updateCursor(NULL, &edit);
                    }
                }
                else if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
                {
                    if(edit.isFraction[edit.activeInput])
                        edit.writtingNumerator[edit.activeInput] = !edit.writtingNumerator[edit.activeInput];
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                updateCursor(&(SDL_Point){event.button.x, event.button.y}, &edit);

                //Check if there is a click on a number
                for(int i = 0; i < 10; i++)
                    if(IsSelected(numberButton[i]))
                        updatePolynomial(numberButton[i]->label[0], &edit);

                //Check if there there is a click on an operator button
                for(int i = 0; i < 4; i++)
                {

                //Check if the reset button was clicked
                    if(IsSelected(operatorButton[i]))//If once of these button was selected
                    {
                        switch(i)//Check which was
                        {
                            case 0:
                                updatePolynomial('.', &edit);
                                break;

                            case 1:
                                Fraction_SetValue(edit.polynomial->coefficients[edit.activeInput], Fraction_GetValue(edit.polynomial->coefficients[edit.activeInput]) * -1);
                                break;

                            case 2:
                                edit.isFraction[edit.activeInput] = !edit.isFraction[edit.activeInput];
                                updateCursor(&(SDL_Point){INPUTS_RECTS[edit.activeInput].x + 50, event.button.y}, &edit);
                                break;

                            case 3:
                                updatePolynomial('\b', &edit);
                                break;
                        }
                    }
                }
                if(IsSelected(resetButton))
                {
                    free(polynomial->discriminant);
                    for(int i = 0; i < 3; i++)
                    {
                        Fraction_SetValue(polynomial->coefficients[i], 0);
                        edit.inputsTexts[i][0] = '+'; edit.inputsTexts[i][1] = '0';
                        edit.isFraction[i] = SDL_FALSE;
                        edit.writtingNumerator[i] = SDL_TRUE;
                        if(i < 2)
                        {
                            free(polynomial->roots[i]);
                            polynomial->roots[i] = NULL;
                        }
                    }
                }
                break;
        }

        //Draw background element of the app
        updateRenderer(&edit);

        //Fake an blinking cursor
        flashingCursor(&edit);

        //Update the button
        for(int i = 0; i < 10; i++)
            UpdateButton(numberButton[i], appRenderer, &event);
        for(int i = 0; i < 4; i++)
            UpdateButton(operatorButton[i], appRenderer, &event);
        UpdateButton(resetButton, appRenderer, &event);

        SDL_RenderPresent(appRenderer);
    }

    SDL_StopTextInput();

    //If the program execute even here all done well so,
    exit_statut = EXIT_SUCCESS;

EXIT_TAG:

    for(int i = 0; i < 10; i++)
        FreeButton(numberButton[i]);

    for(int i = 0; i < 4; i++)
        FreeButton(operatorButton[i]);

    FreeButton(resetButton);

    Polynomial_Destroy(polynomial);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(appRenderer);
    SDL_DestroyWindow(appWindow);

    TTF_Quit();
    SDL_Quit();
    return exit_statut;
}
