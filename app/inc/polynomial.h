#ifndef POLYNOME_H_INCLUDED
#define POLYNOME_H_INCLUDED

#include "fraction.h"
#include "Input.h"

#define POLYNOMIAL_GRAPH_W 1000
#define POLYNOMIAL_GRAPH_H 1000

///This structure represent a polynomial graph.
typedef struct
{
    SDL_Rect rect;
    SDL_Point viewStart;
    SDL_Point origin;
} PolynomialGraph;

typedef struct
{
    SDL_bool hasFocus;
    SDL_Rect rect;
    SDL_Point viewStart;
    SDL_Texture *texture;
    SDL_Point totalSize;
} PolynomialResult;

///This structure represent a polynomial.
typedef struct
{
    float coefficients[3];
    float discriminant;
    float roots[2];
    PolynomialResult result;
    PolynomialGraph graph;
} Polynomial;


/**
* @brief Create a new Polynomial.
*
* @param SDL_Rect* resultRect The rectangle where draw the factorization result.
* @param SDL_Rect* graphRect The rectangle where draw the polynomial graph.
*
* @return A pointer to the polynomial create.
*
*/
Polynomial* Polynomial_Create(SDL_Renderer *renderer, SDL_Rect *resultRect, SDL_Rect *graphRect, int charWidth);


/**
* @brief Calculate the discriminant of the polynomial.
*
* @param Polynomial* p The polynomial which the discriminant will be compute.
* @param float* d A pointer to fill the result of the compute in.
*
*/
void Polynomial_ComputeDiscriminant(Polynomial *p, float *d);


/**
* @brief Calculate the roots of the polynomial.
*
* @param Polynomial* p The polynomial which the roots will be compute.
* @param float* r An array where fill the result of the compute.
* @param int n the size of the table.
*
*/
void Polynomial_ComputeRoots(Polynomial *p, float *r, int n);


/**
* @brief Factorize the polynomial by compute the discriminant and the roots.
*
* This function calls implicitly Polynomial_ComputeDiscriminant
* and Polynomial_ComputeRoots
*
* @param Polynomial* p The polynomial which the roots will be compute.
*
*/
void Polynomial_Factorise(Polynomial *p);


/**
* @brief Draw a coefficient of the polynomial in the renderer.
*
* @param Polynomial* p The polynomial which the roots will be compute.
* @param SDL_Renderer* renderer The renderer where draw the coefficient.
* @param int c which coefficient to draw(0:a, 1:b, 2:c)
* @param int posX The position where draw the coefficient.
* @param SDL_bool withX Define if we want to draw also the variable x
*
* @return The right-top position of the coefficient drawn.
*
*/
int Polynomial_DrawCoefficient(Polynomial *p, SDL_Renderer *renderer, TTF_Font *font, int c, int posX, SDL_bool withX);


/**
* @brief Render the factorization result of the polynomial in his result texture.
*
* @param Polynomial* p The polynomial which the result will be render.
* @param SDL_Renderer* renderer The renderer where render the result.
* @param TTF_Font* font The font used to render.
*
*/
void Polynomial_RenderResult(Polynomial *p, SDL_Renderer *renderer, TTF_Font *font);


/**
* @brief Updates the factorization result of the polynomial in the result rectangle.
*
* @param Polynomial* p The polynomial which the result will be render.
* @param SDL_Renderer* renderer The renderer where render the result.
* @param SDL_Event* event Informations about the event that occurs to know how to
*   update the result.
* @param InputEvent* ievent The event occurs in the inputs.
*
*/
void Polynomial_UpdateResult(Polynomial *p, SDL_Renderer *renderer, TTF_Font *font, SDL_Event *event, InputEvent *ievent, SDL_bool wasModified);


/**
* @brief Draw the graph box.
*
* @param Polynomial* p The polynomial which the result will be render.
* @param SDL_Renderer* renderer The renderer where render the graph box.
*
*/
void Polynomial_DrawGraphBox(Polynomial *p, SDL_Renderer *renderer);


/**
* @brief Draw the graph points.
*
* @param Polynomial* p The polynomial which the result will be render.
* @param SDL_Renderer* renderer The renderer where render the graph box.
*
*/
int Polynomial_DrawGraphPoints(Polynomial *p, SDL_Renderer *renderer);


/**
* @brief Update the graph into the result rectangle.
*
* @param Polynomial* p The polynomial which the result will be render.
* @param SDL_Renderer* renderer The renderer where render the result.
* @param SDL_Event* event The event occurs to know how update the graph.
* @param InputEvent* ievent The event occurs in the inputs.
*
*/
int Polynomial_UpdateGraph(Polynomial *p, SDL_Renderer *renderer, SDL_Event *event, InputEvent *ievent);


/**
* @brief Destroy the polynomial.
*
* @param Polynomial* p The polynomial which the result will be render.
*
*/
void Polynomial_Destroy(Polynomial *p);

#endif // POLYNOME_H_INCLUDED
