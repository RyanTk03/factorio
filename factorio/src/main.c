#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../inc/AppButton.h"
#include "../inc/initGraphicalsElements.h"
#include "../inc/const.h"
#include "../inc/mySDLfunc.h"
#include "../inc/Input.h"
#include "../inc/polynomial.h"
#include "../inc/updateRenderer.h"


int main(int argc, char * argv[])
{
    int exit_statut = EXIT_FAILURE;

    if(SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0)
    {
        fprintf(stderr, "main(int, char**): Erreur lors de l'initialisation"
                " de la SDL(%s).", SDL_GetError());
        return exit_statut;
    }

    SDL_Window *appWindow = SDL_CreateWindow("Factorio", SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer *appRenderer = SDL_CreateRenderer(appWindow, -1,
                                                   SDL_RENDERER_ACCELERATED);

    if(appWindow == NULL || appRenderer == NULL)
    {
        fprintf(stderr, "Error, cannot create the window : %s", SDL_GetError());
        goto EXIT_TAG;
    }

    TTF_Font *font = TTF_OpenFont("CourierPrime.ttf", 18);

    if(font == NULL)
    {
        fprintf(stderr, "Error : cannot load the font(CourierPrime.ttf)");
        goto EXIT_TAG;
    }

    SDL_Surface *icon = SDL_LoadBMP("icon.bmp");

    if(icon != NULL)
        SDL_SetWindowIcon(appWindow, icon);

    /*
        We must verify if the font is fixed width because the input system consider
        that it is to display its content. So if it is false, there will be error in
        displaying.
    */
    SDL_Point fontSize;
    if(TTF_FontFaceIsFixedWidth(font))
    {
        TTF_GlyphMetrics(font, '0', NULL, NULL, NULL, NULL, &fontSize.x);
        fontSize.y = TTF_FontHeight(font);
        Input_Init(fontSize);
    }
    else
    {
        fprintf(stderr, "Error : the font uses have not fixed width");
        goto EXIT_TAG;
    }

    Polynomial *polynomial = Polynomial_Create(appRenderer, &INPUT4_RECT, &GRAPH_RECT, fontSize.x);

    InputCursor cursor = {INPUT1_RECT.x + INPUT_WIDTH - INPUT_PADDING,
                           INPUT_Y + (INPUT_HEIGHT / 2) - (CURSOR_HEIGHT / 2),
                           CURSOR_HEIGHT};

    Input *inputs[3];

    AppButton *numberButton[10] = {};
    AppButton *operatorButton[4] = {};
    AppButton *resetButton = NULL;

    SDL_Point buttonsPositions[TOTAL_BUTTONS];

    calculateButtonsPositions(buttonsPositions);

    if(initNumberButton(numberButton, 10, appRenderer, font, buttonsPositions) < 0 ||
       initOperatorButton(operatorButton, 4, appRenderer, font, buttonsPositions) < 0 ||
       initResetButton(&resetButton, appRenderer, font, buttonsPositions) < 0)
    {
        fprintf(stderr ,"Error when initializing the buttons : %s \n", SDL_GetError());
        goto EXIT_TAG;
    }

    if(initInputs(inputs, appRenderer, INPUTS_RECTS, &cursor, polynomial) < 0)
    {
        fprintf(stderr, "Error when initializing the inputs : %s \n", SDL_GetError());
        goto EXIT_TAG;
    }

    SDL_SetRenderDrawColor(appRenderer, BACKGROUND_COLOR_COMPOSANT);
    SDL_RenderClear(appRenderer);

    SDL_RenderPresent(appRenderer);

    SDL_StartTextInput();

    SDL_Event event;
    InputEvent ievent;
    SDL_bool quit = SDL_FALSE;
    while(!quit)
    {
        SDL_WaitEventTimeout(&event, 1000);
        switch(event.type)
        {
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;

            case SDL_KEYDOWN:
            case SDL_TEXTINPUT:
                ievent.type = INPUT_KEYDOWN;
                if(event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_KP_BACKSPACE)
                    ievent.keyValue = INPUT_KBACK;
                else if(event.key.keysym.sym == SDLK_LEFT)
                {
                    ievent.keyValue = INPUT_KLEFT;
                }
                else if(event.key.keysym.sym == SDLK_RIGHT)
                {
                    ievent.keyValue = INPUT_KRIGHT;
                }
                else if(event.key.keysym.sym == SDLK_UP)
                {
                    ievent.keyValue = INPUT_KUP;
                }
                else if(event.key.keysym.sym == SDLK_DOWN)
                {
                    ievent.keyValue = INPUT_KDOWN;
                }
                else if(event.key.keysym.sym == SDLK_f)
                {
                    ievent.keyValue = INPUT_FRACTIONMOD;
                }
                else if(event.key.keysym.sym == SDLK_r)
                {
                    for(int i = 0; i < 3; i++)
                        Input_Reset(inputs[i]);
                }
                else
                    ievent.keyValue = event.text.text[0];

                break;

            case SDL_MOUSEBUTTONDOWN:
                ievent.type = INPUT_MOUSEBUTTONDOWN;
                ievent.mousePosition = (SDL_Point){event.button.x, event.button.y};

                //Check if there is a click on a number
                for(int i = 0; i < 10; i++)
                    if(IsSelected(numberButton[i])) {
                        ievent.type = INPUT_KEYDOWN;
                        ievent.keyValue = numberButton[i]->label[0];
                    }

                //Check if there there is a click on an operator button
                for(int i = 0; i < 4; i++)
                {
                    if(IsSelected(operatorButton[i]))//If once of these button was selected
                    {
                        switch(i)//Check which was
                        {
                            case 0:
                                ievent.type = INPUT_KEYDOWN;
                                ievent.keyValue = '.';
                                break;

                            case 1:
                                ievent.type = INPUT_KEYDOWN;
                                ievent.keyValue = '-';
                                break;

                            case 2:
                                ievent.type = INPUT_KEYDOWN;
                                ievent.keyValue = INPUT_FRACTIONMOD;
                                break;

                            case 3:
                                ievent.type = INPUT_KEYDOWN;
                                ievent.keyValue = INPUT_KBACK;
                                break;
                        }
                    }
                }
                //Check if the reset button was clicked
                if(IsSelected(resetButton))
                {
                    for(int i = 0; i < 3; i++)
                        Input_Reset(inputs[i]);
                }

                //Check if another input was clicked
                for(int i = 0; i < 3; i++)
                {
                    if(SDL_PointInRect(&ievent.mousePosition, &inputs[i]->rect))
                    {
                        ievent.hasFocus = inputs[i];
                        break;
                    }
                    else
                        ievent.hasFocus = NULL;
                }
                break;

            default:
                ievent.type = INPUT_NOEVENT;
                break;
        }

        //Draw background element of the app
        updateRenderer(appRenderer, font);

        //Update the button
        for(int i = 0; i < 10; i++)
            UpdateButton(numberButton[i], appRenderer, &event);
        for(int i = 0; i < 4; i++)
            UpdateButton(operatorButton[i], appRenderer, &event);
        UpdateButton(resetButton, appRenderer, &event);

        //Update the inputs
        for(int i = 0; i < 3; i++)
            Input_Update(inputs[i], appRenderer, font, &ievent);

        //Update the polynomial
        Polynomial_UpdateResult(polynomial, appRenderer, font, &event, &ievent,
            inputs[0]->dataWasModified || inputs[1]->dataWasModified || inputs[2]->dataWasModified);
        Polynomial_UpdateGraph(polynomial, appRenderer, &event, &ievent);//Update the graph

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

    if(font != NULL)
        TTF_CloseFont(font);

    if(icon != NULL)
        SDL_FreeSurface(icon);

    if(appRenderer != NULL)
        SDL_DestroyRenderer(appRenderer);

    if(appWindow != NULL)
        SDL_DestroyWindow(appWindow);

    TTF_Quit();
    SDL_Quit();
    return exit_statut;
}
