#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../inc/AppButton.h"
#include "../inc/mySDLfunc.h"

AppButton* CreateButton(int w, int h, const char *label, TTF_Font *font)
{
    //Creation of an instance of the button//
    AppButton *button = malloc(sizeof(*button));
    if(button == NULL)
    {
        SDL_SetError("Cannot create an instance of a button :: CreateButton().\n");
        return NULL;
    }

    //The font use to draw the label in the button//
    button->font = font;
    if(font == NULL)
    {
        SDL_SetError("La police n'a pas pu etre creer :: CreateButton().\n");
        button->font = TTF_OpenFont("C/Windows/Fonts/arial.ttf", 12);
        if(button->font == NULL)
        {
            free(button);
            return NULL;
        }
    }

    //Set the label of the button//
    if(label == NULL)
    {
        SDL_SetError("The label of the button is empty :: BTN_CreateButton().\n");
        label = " ";
    }
    if(strlen(label) > MAX_LABEL_SIZE - 1)
        SDL_SetError("The label of the button is to long :: BTN_CreateButton().\n");

    snprintf(button->label, MAX_LABEL_SIZE, label);

    button->rect.w = w;
    button->rect.h = h;

    /*
    * The colors of the buttons
    */

    //The color of the label in the button
    button->labelColor.r = 0;
    button->labelColor.g = 0;
    button->labelColor.b = 0;
    button->labelColor.a = 255;

    //The background color of the button
    button->defaultColor.r = 0;
    button->defaultColor.g = 162;
    button->defaultColor.b = 232;
    button->defaultColor.a = 255;

    //The background color of the button on hover
    button->hoveredColor.r = 0;
    button->hoveredColor.g = 0;
    button->hoveredColor.b = 0;
    button->hoveredColor.a = 25;

    //The background color of the button on hover
    button->activeColor.r = 0;
    button->activeColor.g = 0;
    button->activeColor.b = 0;
    button->activeColor.a = 25;

    return button;
}

int CopyButton(AppButton *button, SDL_Renderer *renderer, SDL_Point position)
{
    SDL_Texture *renderTarget = SDL_GetRenderTarget(renderer);//save the default renderer target

    button->rect.x = position.x;
    button->rect.y = position.y;

    //Creation of the texture of the button
    button->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, button->rect.w * 3, button->rect.h);
    if(button->texture == NULL)
    {
        SDL_SetError("Cannot textured the button :: CopyButton()\n");
        return -1;
    }
    if(SDL_SetTextureBlendMode(button->texture, SDL_BLENDMODE_BLEND) < 0)
        SDL_SetError("Cannot apply transparency to the button texture :: CopyButton()\n");


    /*
    * First of all, let's draw the button at his normal state
    */
    //set the render target to a temp texture
    if(SDL_SetRenderTarget(renderer, button->texture) < 0)
    {
        SDL_SetError("Cannot textured the button :: CopyButton()\n");
        SDL_DestroyTexture(button->texture);
        return -1;
    }
    if(SDL_SetRenderDrawColor(renderer, button->defaultColor.r, button->defaultColor.g, button->defaultColor.b,
                              button->defaultColor.a) < 0 ||
       SDL_RenderClear(renderer) < 0)
        SDL_SetError("Cannot clear the button :: CopyButton()\n");

    //fill the button texture with normal background color at the corresponding rect
    if(SDL_SetRenderDrawColor(renderer, button->defaultColor.r, button->defaultColor.g, button->defaultColor.b,
                              button->defaultColor.a) < 0 ||
       SDL_RenderFillRect(renderer, &(SDL_Rect){0, 0, button->rect.w, button->rect.h}))
        SDL_SetError("Cannot set default color to the button :: CopyButton()\n");

    //fill the button texture with hovered background color at the corresponding rect
    if(SDL_SetRenderDrawColor(renderer, button->hoveredColor.r, button->hoveredColor.g, button->hoveredColor.b,
                              button->hoveredColor.a) < 0 ||
       SDL_RenderFillRect(renderer, &(SDL_Rect){button->rect.w, 0, button->rect.w, button->rect.h}))
        SDL_SetError("Cannot set hover color to the button :: CopyButton()\n");

    //fill the button texture with the active(click) background color at the corresponding rect
    if(SDL_SetRenderDrawColor(renderer, button->activeColor.r, button->activeColor.g, button->activeColor.b,
                              button->activeColor.a) < 0 ||
       SDL_RenderFillRect(renderer, &(SDL_Rect){button->rect.w * 2, 0, button->rect.w, button->rect.h}))
        SDL_SetError("Cannot set active color to the button :: CopyButton()\n");

    //Creation of the label's texture on normal state
    SDL_Rect labelRect;
    SDL_Texture *temp = MyTTF_RenderText_Blended(renderer, button->font, button->label, button->labelColor,
                                                 &labelRect.w, &labelRect.h);
    if(temp == NULL)
    {
        SDL_SetError("Cannot create a label to the button:: CopyButton().\n");
        SDL_SetRenderTarget(renderer, renderTarget);
        SDL_DestroyTexture(button->texture);
        return -1;
    }
    labelRect.x = (button->rect.w/2) - (labelRect.w/2);
    labelRect.y = (button->rect.h/2) - (labelRect.h/2);

    //draw the label on the default state part of the texture
    if(SDL_RenderCopy(renderer, temp, NULL, &labelRect) < 0)
        SDL_SetError("Cannot set a label to the button:: CopyButton().\n");

    //draw the label on the active state part of the texture
    labelRect.x += 2 * button->rect.w;
    if(SDL_RenderCopy(renderer, temp, NULL, &labelRect) < 0)
        SDL_SetError("Cannot set a label to the button:: CopyButton().\n");

    //draw the label on the hovered state part of the texture
    labelRect.w += 1;
    labelRect.h += 1;
    labelRect.x = button->rect.w + (button->rect.w/2) - (labelRect.w/2);
    labelRect.y = (button->rect.h/2) - (labelRect.h/2);
    if(SDL_RenderCopy(renderer, temp, NULL, &labelRect) < 0)
        SDL_SetError("Cannot set a label to the button:: CopyButton().\n");
    SDL_DestroyTexture(temp);

    //reset the default renderer target
    SDL_SetRenderTarget(renderer, renderTarget);
    if(SDL_RenderCopy(renderer, button->texture, &(SDL_Rect){0, 0, button->rect.w, button->rect.h}, &button->rect) < 0)
        SDL_SetError("Cannot copy the button to the renderer:: CopyButton().\n");

    button->rectToDraw = (SDL_Rect){0, 0, button->rect.w, button->rect.h};

    return 0;
}

int SetButtonColor(AppButton *button, SDL_Renderer *renderer, SDL_Color color, ButtonColor colorType)
{
    SDL_Texture *renderTarget = SDL_GetRenderTarget(renderer);//save the default renderer target
    SDL_SetRenderTarget(renderer, button->texture);

    //Creation of the label's texture on normal state
    if(colorType == LABEL_COLOR)
        button->labelColor = color;

    SDL_Rect labelRect;
    SDL_Texture *temp = MyTTF_RenderText_Blended(renderer, button->font, button->label, button->labelColor,
                                                 &labelRect.w, &labelRect.h);

    if(temp == NULL)
    {
        SDL_SetError("Cannot create the texture of the label :: CopyButton().\n");
        return -1;
    }
    labelRect.x = (button->rect.w/2) - (labelRect.w/2);
    labelRect.y = (button->rect.h/2) - (labelRect.h/2);

    //update the color
    switch(colorType)
    {
        case DEFAULT_COLOR:
            button->defaultColor = color;
            SDL_SetRenderDrawColor(renderer, button->defaultColor.r, button->defaultColor.g, button->defaultColor.b, button->defaultColor.a);
            SDL_RenderFillRect(renderer, &(SDL_Rect){0, 0, button->rect.w, button->rect.h});
            SDL_RenderPresent(renderer);
            break;
        case HOVER_COLOR:
            button->hoveredColor = color;
            SDL_SetRenderDrawColor(renderer, button->hoveredColor.r, button->hoveredColor.g, button->hoveredColor.b, button->hoveredColor.a);
            SDL_RenderFillRect(renderer, &(SDL_Rect){button->rect.w, 0, button->rect.w, button->rect.h});
            SDL_RenderPresent(renderer);
            break;
        case ACTIVE_COLOR:
            button->activeColor = color;
            SDL_SetRenderDrawColor(renderer, button->activeColor.r, button->activeColor.g, button->activeColor.b, button->activeColor.a);
            SDL_RenderFillRect(renderer, &(SDL_Rect){button->rect.w * 2, 0, button->rect.w, button->rect.h});
            SDL_RenderPresent(renderer);
            break;
    }

    //update the label
    switch(colorType)
    {
        case DEFAULT_COLOR:
            SDL_RenderCopy(renderer, temp, NULL, &labelRect);
            SDL_RenderPresent(renderer);
            break;
        case HOVER_COLOR:
            labelRect.w += 1;
            labelRect.h += 1;
            labelRect.x = button->rect.w + (button->rect.w/2) - (labelRect.w/2);
            labelRect.y = (button->rect.h/2) - (labelRect.h/2);
            SDL_RenderCopy(renderer, temp, NULL, &labelRect);
            SDL_RenderPresent(renderer);
            break;
        case ACTIVE_COLOR:
            labelRect.x += (2 * button->rect.w);
            SDL_RenderCopy(renderer, temp, NULL, &labelRect);
            SDL_RenderPresent(renderer);
            break;
        case LABEL_COLOR:
            //draw the label on the default state part of the texture
            SDL_RenderCopy(renderer, temp, NULL, &labelRect);
            SDL_RenderPresent(renderer);
            //draw the label on the active state part of the texture
            labelRect.x += 2 * button->rect.w;
            SDL_RenderCopy(renderer, temp, NULL, &labelRect);
            SDL_RenderPresent(renderer);
            //draw the label on the hovered state part of the texture
            labelRect.w += 1;
            labelRect.h += 1;
            labelRect.x = button->rect.w + (button->rect.w/2) - (labelRect.w/2);
            labelRect.y = (button->rect.h/2) - (labelRect.h/2);
            SDL_RenderCopy(renderer, temp, NULL, &labelRect);
            SDL_RenderPresent(renderer);
            break;

    }

    SDL_SetRenderTarget(renderer, renderTarget);

    SDL_DestroyTexture(temp);

    return 0;
}

int UpdateButton(AppButton *button, SDL_Renderer *renderer, SDL_Event *event)
{
    if(button == NULL)
        return -1;

    SDL_Point cursor = {event->button.x, event->button.y};
    if(SDL_PointInRect(&cursor, &button->rect))
    {
        if(event->type == SDL_MOUSEBUTTONDOWN)
            button->rectToDraw.x = button->rect.w * 2;
        else
            button->rectToDraw.x = button->rect.w;
    }
    else if(event->type == SDL_MOUSEMOTION)
        button->rectToDraw.x = 0;

    SDL_RenderCopy(renderer, button->texture, &button->rectToDraw, &button->rect);

    return 0;
}

SDL_bool IsHovered(AppButton *button)
{/*
    if(button == NULL)
        return SDL_FALSE;

    if(button->hovered)
    {
        button->hovered = SDL_FALSE;
        return SDL_TRUE;
    }
*/
    return SDL_FALSE;
}

SDL_bool IsSelected(AppButton *button)
{
    if(button == NULL)
        return SDL_FALSE;

    SDL_Point cursor;
    SDL_GetMouseState(&cursor.x, &cursor.y);

    if(SDL_PointInRect(&cursor, &button->rect))
        return SDL_TRUE;

    return SDL_FALSE;
}

void FreeButton(AppButton *button)
{
    if(button != NULL)
    {
        if(button->texture != NULL)
            SDL_DestroyTexture(button->texture);
        free(button);
    }
}
