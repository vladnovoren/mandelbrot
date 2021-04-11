#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP


#include <SDL2/SDL.h>
#include <stdint.h>
#include <immintrin.h>
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

static const int    HALF_SCREEN_WIDTH  = SCREEN_WIDTH / 2;
static const int    HALF_SCREEN_HEIGHT = SCREEN_HEIGHT / 2;


static const int    MAX_N_ITERATION = 255;
static const double MAX_RADIUS      = 100.0;
static const double DEFAULT_DELTA_Y = (double)4 / SCREEN_HEIGHT;
static const double DEFAULT_DELTA_X = DEFAULT_DELTA_Y * 4;

static const double INC_SCALE_MUL = 2.0;
static const double DEC_SCALE_MUL = 1.0 / INC_SCALE_MUL;

static const char   *MANDELBROT_TITLE = "Mandelbrot";
 

static const __m256d _V_COEFFICIENTS = _mm256_set_pd(3.0, 2.0, 1.0, 0.0);
static const __m256d _V_FOUR         = _mm256_set1_pd(4.0);

static const __m256d _V_SCREEN_WIDTH  = _mm256_set1_pd(SCREEN_WIDTH);
static const __m256d _V_SCREEN_HEIGHT = _mm256_set1_pd(SCREEN_HEIGHT);


static const __m256d _V_HALF_SCREEN_WIDTH  = _mm256_set1_pd(HALF_SCREEN_WIDTH);
static const __m256d _V_HALF_SCREEN_HEIGHT = _mm256_set1_pd(HALF_SCREEN_HEIGHT);

static const __m256d _V_DEFAULT_DELTA_X = _mm256_set1_pd(DEFAULT_DELTA_X);
static const __m256d _V_DEFAULT_DELTA_Y = _mm256_set1_pd(DEFAULT_DELTA_Y);

static const __m256d _V_DEFAULT_START_X = _mm256_mul_pd(_mm256_sub_pd(_V_COEFFICIENTS, _V_HALF_SCREEN_WIDTH), _V_DEFAULT_DELTA_Y);
static const __m256d _V_DEFAULT_START_Y = _mm256_mul_pd(_V_HALF_SCREEN_HEIGHT, _V_DEFAULT_DELTA_Y);

static const __m256d _V_INC_SCALE_MUL = _mm256_set1_pd(INC_SCALE_MUL);
static const __m256d _V_DEC_SCALE_MUL = _mm256_set1_pd(DEC_SCALE_MUL);

static const __m256d _V_D_NULL = _mm256_set1_pd(0);
static const __m256i _V_I_NULL = _mm256_set1_epi64x(0);

static const __m256d _V_MAX_RADIUS = _mm256_set1_pd(MAX_RADIUS);

struct WinWithSurf {
    SDL_Window  *window;
    SDL_Surface *surface;
};

struct App {
    WinWithSurf main_win;

    __m256d     _v_start_x;
    __m256d     _v_start_y;

    __m256d     _v_delta_x;
    __m256d     _v_delta_y;
};


int InitSDL();

int InitMainWin(WinWithSurf *main_win);

void DestructWinWithSurf(WinWithSurf *main_win);

int InitApp(App *app);

void DestructApp(App *app); 

__m256i CountColor(double x0, double y0); 

void FillMandelbrot(App *app);

void ExecApp();


#endif /* mandelbrot.hpp  */

