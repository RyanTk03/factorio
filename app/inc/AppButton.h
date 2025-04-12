#ifndef APPBUTTON_H_INCLUDED
#define APPBUTTON_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MAX_LABEL_SIZE 12

typedef struct
{
    SDL_Texture *texture;
    TTF_Font *font;
    SDL_Rect rect;
    SDL_Rect rectToDraw;
    SDL_Color defaultColor;
    SDL_Color hoveredColor;
    SDL_Color activeColor;
    SDL_Color labelColor;
    char label[MAX_LABEL_SIZE];
} AppButton;

typedef enum
{
    DEFAULT_COLOR, HOVER_COLOR, LABEL_COLOR, ACTIVE_COLOR
} ButtonColor;

SDL_bool IsSelected(AppButton *button);
AppButton* CreateButton(int w, int h, const char *label, TTF_Font *font);
int CopyButton(AppButton *button, SDL_Renderer *renderer, SDL_Point position);
int SetButtonColor(AppButton *button, SDL_Renderer *renderer, SDL_Color color, ButtonColor colorType);
int UpdateButton(AppButton *button, SDL_Renderer *renderer, SDL_Event *event);
void FreeButton(AppButton *button);

#endif // APPBUTTON_H_INCLUDED
