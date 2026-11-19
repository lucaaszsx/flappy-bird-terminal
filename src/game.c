#include "game.h"
#include "util.h"
#include "window.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void pipe_append(game_t *g, size_t pipe_idx);
static void pipe_draw(game_t *g, size_t pipe_idx);

void game_init(game_t *g, win_t *win) {
    g->win = win;
    g->last_pipe_frame = (struct timespec){0};

    for (size_t k = 0; k < NUM_PIPES; k++)
        pipe_append(g, k);
}

void game_refresh(win_t last_win, win_t *current_win) {}

void game_tick(game_t *g, struct timespec current_frame) {
    clock_gettime(CLOCK_MONOTONIC, &current_frame);

    win_draw_color(g->win, CL_BLUE);
    win_frect(g->win, 0, 0, g->win->w, g->win->h);

    bool update_pipes = delta_time(current_frame, g->last_pipe_frame) >= NS_IN_SEC / 36;
    if (update_pipes) g->last_pipe_frame = current_frame;

    win_draw_color(g->win, CL_GREEN);
    for (size_t k = 0; k < NUM_PIPES; k++) {
        pipe_draw(g, k);

        if (update_pipes) {
            if (g->pipes[k].x > 0) g->pipes[k].x -= normalize(1, g->win->w);
        }
    }

    win_flush();
}

static int get_pipe_tp_size() {
    assert(PIPE_MAX_HEIGHT - PIPE_MIN_HEIGHT > 0 &&
            "maximum pipe height must be greater than minimum height");
    return PIPE_MIN_HEIGHT + rand() % (PIPE_MAX_HEIGHT - PIPE_MIN_HEIGHT);
}

static void pipe_append(game_t *g, size_t pipe_idx) {
    g->pipes[pipe_idx] = (pipe_t){
        .x=1, // starts outside window width
        .top_size=normalize(get_pipe_tp_size(), g->win->h), // gets a random value to be the height of the top pipe
        .w=normalize(PIPE_WIDTH, g->win->w)
    };
}

static void pipe_draw(game_t *g, size_t pipe_idx) {
    pipe_t pipe = g->pipes[pipe_idx];

    win_draw_color(g->win, CL_GREEN);

    double gap = normalize(PIPE_Y_GAP, g->win->h);
    int x_normal = denormalize(pipe.x, g->win->w);
    int w_normal = denormalize(pipe.w, g->win->w);
    int top_h_normal = denormalize(pipe.top_size, g->win->h);

    // draws the top part of the pipe
    win_frect(g->win,
                x_normal, 0,
                w_normal, top_h_normal);

    // draws the bottom part of the pipe
    win_draw_color(g->win, CL_RED);
    win_frect(g->win,
        x_normal, top_h_normal + denormalize(gap, g->win->h),
        w_normal, 100);
}
