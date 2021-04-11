#include "../include/mandelbrot.hpp"


int InitSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}


int InitMainWin(WinWithSurf *main_win) {
	if (!(main_win->window = SDL_CreateWindow(MANDELBROT_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0))) {
        REPORT_SDL_ERROR
        return 1;
    }

    if (!(main_win->surface = SDL_GetWindowSurface(main_win->window))) {
        REPORT_SDL_ERROR
        return 1;
    }

    return 0;
}


void DestructWinWithSurf(WinWithSurf *main_win) {
    SDL_FreeSurface(main_win->surface);
    SDL_DestroyWindow(main_win->window);
}


int InitApp(App *app) {
    assert(app);

    if (InitMainWin(&app->main_win)) {
        return 1;
    }

    app->_v_delta_x = _V_DEFAULT_DELTA_X;
    app->_v_delta_y = _V_DEFAULT_DELTA_Y;

    app->_v_start_x = _V_DEFAULT_START_X;
    app->_v_start_y = _V_DEFAULT_START_Y;
    return 0;
}


void DestructApp(App *app) {
    assert(app);

    DestructWinWithSurf(&app->main_win);
}


__m256i CountColor(__m256d _v_x0, __m256d _v_y0) {
    __m256d _v_x    = _v_x0,
            _v_y    = _v_y0,
           _v_sqr_x = _V_D_NULL,
           _v_sqr_y = _V_D_NULL,
         _v_x_mul_y = _V_D_NULL;

    int n_iterations = 0;
    __m256i _v_n_iterations = _V_I_NULL;
    while (n_iterations < MAX_N_ITERATION) {
        _v_sqr_x = _mm256_mul_pd(_v_x, _v_x);
        _v_sqr_y = _mm256_mul_pd(_v_y, _v_y);

        __m256d _v_cmp_res = _mm256_cmp_pd(_mm256_add_pd(_v_sqr_x, _v_sqr_y), _V_MAX_RADIUS, _CMP_LE_OQ);
        __m256i _cast_res  = _mm256_castpd_si256(_v_cmp_res);
        int moved_mask = _mm256_movemask_pd(_v_cmp_res);
        if (!moved_mask) {
            break;
        }

        _v_x_mul_y = _mm256_mul_pd(_v_x, _v_y);
        _v_x       = _mm256_add_pd(_mm256_sub_pd(_v_sqr_x, _v_sqr_y), _v_x0);
        _v_y       = _mm256_add_pd(_mm256_add_pd(_v_x_mul_y, _v_x_mul_y), _v_y0);

        _v_n_iterations = _mm256_sub_epi64(_v_n_iterations, _cast_res);
        n_iterations++;
    }

    return _v_n_iterations;
}


void FillMandelbrot(App *app) {
    uint32_t *pixels = (uint32_t *)app->main_win.surface->pixels;

    __m256d _v_x = app->_v_start_x, _v_y = app->_v_start_y;

    for (int p_y = 0; p_y < SCREEN_HEIGHT; p_y++) {
        _v_x = app->_v_start_x;
        for (int p_x = 0; p_x < SCREEN_WIDTH; p_x += 4) {
            __m256i _v_colors = CountColor(_v_x, _v_y);
            for (int i = 0; i < 4; i++) {
                pixels[i] = _v_colors[i];
            }
            _v_x = _mm256_add_pd(_v_x, app->_v_delta_x);
            pixels += 4;
        }
        _v_y = _mm256_sub_pd(_v_y, app->_v_delta_y);
    }
}



void ExecApp() {
    App app = {};
    InitApp(&app);

    time_t cur_time = 0;
    bool quit = false;
    SDL_Event event = {};
    while (!quit) {
        cur_time = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            app._v_start_x = _mm256_sub_pd(app._v_start_x, _mm256_mul_pd(_V_HALF_SCREEN_WIDTH,  app._v_delta_y));
                            break;
                        case SDLK_RIGHT:
                            app._v_start_x = _mm256_add_pd(app._v_start_x, _mm256_mul_pd(_V_HALF_SCREEN_WIDTH,  app._v_delta_y));
                            break;
                        case SDLK_UP:
                            app._v_start_y = _mm256_add_pd(app._v_start_y, _mm256_mul_pd(_V_HALF_SCREEN_HEIGHT, app._v_delta_y));
                            break;
                        case SDLK_DOWN:
                            app._v_start_y = _mm256_sub_pd(app._v_start_y, _mm256_mul_pd(_V_HALF_SCREEN_HEIGHT, app._v_delta_y));
                            break;
                        case SDLK_LSHIFT:
                            app._v_start_x = _mm256_sub_pd(app._v_start_x, _mm256_mul_pd(_V_HALF_SCREEN_WIDTH,  app._v_delta_y));
                            app._v_start_x = _mm256_sub_pd(app._v_start_x, _mm256_mul_pd(_V_COEFFICIENTS, app._v_delta_y));

                            app._v_start_y = _mm256_add_pd(app._v_start_y, _mm256_mul_pd(_V_HALF_SCREEN_HEIGHT, app._v_delta_y));

                            app._v_delta_x = _mm256_mul_pd(app._v_delta_x, _V_INC_SCALE_MUL);
                            app._v_delta_y = _mm256_mul_pd(app._v_delta_y, _V_INC_SCALE_MUL);

                            app._v_start_x = _mm256_add_pd(app._v_start_x, _mm256_mul_pd(_V_COEFFICIENTS, app._v_delta_y));
                            break;
                        case SDLK_RSHIFT:
                            app._v_start_x = _mm256_sub_pd(app._v_start_x, _mm256_mul_pd(_V_COEFFICIENTS, app._v_delta_y));

                            app._v_delta_x = _mm256_mul_pd(app._v_delta_x, _V_DEC_SCALE_MUL);
                            app._v_delta_y = _mm256_mul_pd(app._v_delta_y, _V_DEC_SCALE_MUL);

                            app._v_start_x = _mm256_add_pd(app._v_start_x, _mm256_mul_pd(_V_HALF_SCREEN_WIDTH,  app._v_delta_y));
                            app._v_start_y = _mm256_sub_pd(app._v_start_y, _mm256_mul_pd(_V_HALF_SCREEN_HEIGHT, app._v_delta_y));

                            app._v_start_x = _mm256_add_pd(app._v_start_x, _mm256_mul_pd(_V_COEFFICIENTS, app._v_delta_y));
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        FillMandelbrot(&app);
        SDL_UpdateWindowSurface(app.main_win.window);
        printf("FPS: %lf\n", 1000.0 / (SDL_GetTicks() - cur_time));
    }
    DestructApp(&app);
}

