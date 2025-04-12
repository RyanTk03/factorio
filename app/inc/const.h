#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

//Screen constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 540
#define BACKGROUND_COLOR (SDL_Color){63, 72, 204, 255}
#define BACKGROUND_COLOR_COMPOSANT 63, 72, 204, 255

//The inputs constants
#define INPUT_MARGIN 22
#define INPUT_PADDING 5
#define INPUT_WIDTH 70
#define INPUT_HEIGHT 40
#define INPUT1_X 16
#define INPUT_Y 25
#define INPUT1_RECT (SDL_Rect){INPUT1_X, INPUT_Y, INPUT_WIDTH, INPUT_HEIGHT}
#define INPUT2_RECT (SDL_Rect){INPUT1_RECT.x + INPUT1_RECT.w + INPUT_MARGIN, INPUT_Y, INPUT_WIDTH, INPUT_HEIGHT}
#define INPUT3_RECT (SDL_Rect){INPUT2_RECT.x + INPUT2_RECT.w + INPUT_MARGIN, INPUT_Y, INPUT_WIDTH, INPUT_HEIGHT}
#define INPUT4_RECT (SDL_Rect){INPUT3_RECT.x + INPUT3_RECT.w + INPUT_MARGIN, INPUT_Y, INPUT_WIDTH * 3, INPUT_HEIGHT}
#define INPUTS_RECTS (SDL_Rect[]){INPUT1_RECT, INPUT2_RECT, INPUT3_RECT, INPUT4_RECT}

//The inputs cursor constants
#define CURSOR_STARTING_X(rect) rect.x + rect.w - INPUT_PADDING
#define CURSOR_HEIGHT 22
#define CURSOR_FRACTION_HEIGHT 14
#define CURSOR_Y 22

//The labels constants
#define LABEL_XSQUARE (SDL_Point){14, 19}
#define LABEL_X (SDL_Point){8, 19}
#define LABEL_EQUAL (SDL_Point){11, 19}

//The graphics constants
#define GRAPH_WIDTH SCREEN_WIDTH
#define GRAPH_HEIGHT 200
#define GRAPH_X 0
#define GRAPH_Y 85
#define GRAPH_ORIGIN_X GRAPH_X + (GRAPH_WIDTH / 2)
#define GRAPH_ORIGIN_Y GRAPH_Y + (GRAPH_HEIGHT / 2)
#define GRAPH_BORDER_COLOR (SDL_Color){195, 195, 195, 255}
#define GRAPH_BORDER_COLOR_COMPOSANT 195, 195, 195, 255
#define GRAPH_RECT (SDL_Rect){GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT}

#define NUMBERBUTTON_W 160
#define NUMBERBUTTON_H 60

#define TOTAL_BUTTONS 15
#define TOTAL_GROUP1_BUTTONS 14 //the buttons of group1 is the button in composed of the numerics, the decimal and fraction button
#define TOTAL_GROUP2_BUTTONS 2 //the buttons "factoriser" and "reset"

#define MAXNUMBERCHAR 10


#endif // CONST_H_INCLUDED
