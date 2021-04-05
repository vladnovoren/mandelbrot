#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP


#include <SDL2/SDL.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


#define REPORT_SDL_ERROR \
printf("in file %s, function %s, line %d:\n\t", __FILE__, __PRETTY_FUNCTION__, __LINE__); \
printf("%s\n", SDL_GetError());


static const int    SCREEN_WIDTH    = 800;
static const int    SCREEN_HEIGHT   = 600;

static const int    HALF_SCREEN_WIDTH = SCREEN_WIDTH / 2;
static const int    HALF_SCREEN_HEIGHT = SCREEN_HEIGHT / 2;


static const int    MAX_N_ITERATION = 255;
static const double MAX_RADIUS      = 100.0;
static const double DEFAULT_SCALE   = (double)4 / SCREEN_HEIGHT;

static const double START_Y         = (double)HALF_SCREEN_HEIGHT * DEFAULT_SCALE;
static const double START_X         = (double)-HALF_SCREEN_WIDTH * DEFAULT_SCALE;

static const char   *MANDELBROT_TITLE = "Mandelbrot";



struct WinWithSurf {
    SDL_Window  *window;
    SDL_Surface *surface;
};

struct App {
    WinWithSurf main_win;
    double      scale;
    double      x0;
    double      y0;
};


int InitSDL();

int InitMainWin(WinWithSurf *main_win);

void DestructWinWithSurf(WinWithSurf *main_win);

int InitApp(App *app);

void DestructApp(App *app); 

uint32_t CountColor(double x0, double y0); 

void FillMandelbrot(App *app);

void ExecApp();


#endif /* mandelbrot.hpp  */

