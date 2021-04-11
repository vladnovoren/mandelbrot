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
    app->scale = DEFAULT_SCALE;
    app->x0 = START_X;
    app->y0 = START_Y;
    return 0;
}


void DestructApp(App *app) {
    assert(app);

    DestructWinWithSurf(&app->main_win);
}


uint32_t CountColor(double x0, double y0) {
    double x     = x0,
           y     = y0,
           sqr_x = 0,
           sqr_y = 0,
         x_mul_y = 0;

    int n_iterations = 0;
    while (n_iterations < MAX_N_ITERATION) {
        sqr_x   = x * x;
        sqr_y   = y * y;
        if (sqr_x + sqr_y > MAX_RADIUS) {
            break;
        }

        x_mul_y = x * y;
        x       = sqr_x - sqr_y + x0;
        y       = 2 * x_mul_y + y0;

        n_iterations++;
    }

    return 0 | (n_iterations * n_iterations) | (n_iterations<<16) | (n_iterations<<24);
}


void FillMandelbrot(App *app) {
    uint32_t *pixels = (uint32_t *)app->main_win.surface->pixels;

    double x = app->x0, y = app->y0;

    for (int p_y = 0; p_y < SCREEN_HEIGHT; p_y++) {
        x = app->x0;
        for (int p_x = 0; p_x < SCREEN_WIDTH; p_x++) {
            *pixels = CountColor(x, y);
            x += app->scale;
            pixels++;
        }
        y -= app->scale;
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
                            app.x0 -= HALF_SCREEN_WIDTH * app.scale;
                            break;
                        case SDLK_RIGHT:
                            app.x0 += HALF_SCREEN_WIDTH * app.scale;
                            break;
                        case SDLK_UP:
                            app.y0 += HALF_SCREEN_HEIGHT * app.scale;
                            break;
                        case SDLK_DOWN:
                            app.y0 -= HALF_SCREEN_HEIGHT * app.scale;
                            break;
                        case SDLK_LSHIFT:
                            app.x0 -= app.scale * HALF_SCREEN_WIDTH;
                            app.y0 += app.scale * HALF_SCREEN_HEIGHT;
                            app.scale *= 2;
                            break;
                        case SDLK_RSHIFT:
                            app.scale /= 2;
                            app.x0 += app.scale * HALF_SCREEN_WIDTH;
                            app.y0 -= app.scale * HALF_SCREEN_HEIGHT;
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

