#include "game.h"
#include "window.h"
#include "util.h"
#include <bits/time.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#define TOTAL_PIPES 3

// clear -> draw bg -> draw bird -> draw pipe

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Pipe;

const int width = 10;
const int height = 10;
Pipe pipe[TOTAL_PIPES];
struct timespec last_pipe_frame = {0,0};
struct timespec last_bird_frame = {0,0};
bool game_over = false;
const win_color_t cls[TOTAL_PIPES]={CL_RED,CL_GREEN,CL_YELLOW};

void game_init(win_t *win) {
    for (size_t k = 0; k < TOTAL_PIPES; k++) {
        pipe[k].x = win->w - k * 11 - 1;
        pipe[k].y = win->h - 1;
        pipe[k].w = 10;
        pipe[k].h = 10;
    }
}

void game_refresh(win_t last_win, win_t *current_win) {
    double scale_x = (double) current_win->w / last_win.w;
    double scale_y = (double) current_win->h / last_win.h;

    for (size_t k = 0; k < TOTAL_PIPES; k++) {
        pipe[k].x = round((double)pipe[k].x * scale_x);
        pipe[k].y = round((double)pipe[k].y * scale_y);
    }
}

void game_tick(win_t *win, struct timespec current_frame) {
    if (game_over) {
        win_draw_color(win, CL_RED);
        win_frect(win, 0, 0, win->w, win->h);
        win_flush();
        return;
    }

    clock_gettime(CLOCK_MONOTONIC, &current_frame);

    win_draw_color(win, CL_BLUE);
    win_frect(win, 0, 0, win->w, win->h);

    bool update_pipes = delta_time(current_frame, last_pipe_frame) >= NS_IN_SEC / 36;
    if (update_pipes) last_pipe_frame = current_frame;

    for (size_t k = 0; k < TOTAL_PIPES; k++) {
        win_draw_color(win, cls[k]);
        win_frect(win, pipe[k].x, pipe[k].y, pipe[k].w, pipe[k].h);

        if (update_pipes) {
            if (pipe[k].x <= -pipe[k].w) pipe[k].x = win->w;
            else pipe[k].x -= 1;
        }
    }

    win_flush();
}
