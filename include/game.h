#pragma once

#include "window.h"
#include <stddef.h>
#include <time.h>

#define PIPE_WIDTH 10
#define PIPE_MAX_HEIGHT 10
#define PIPE_MIN_HEIGHT 5
#define PIPE_X_GAP 10
#define PIPE_Y_GAP 10
#define NUM_PIPES 3

typedef struct {
    double x;
    double top_size;
    double w;
} pipe_t;

typedef struct {
    win_t *win;
    pipe_t pipes[NUM_PIPES];
    struct timespec last_pipe_frame;
} game_t;

void game_init(game_t *g, win_t *win);
void game_refresh(win_t last_win, win_t *current_win);
void game_tick(game_t *g, struct timespec current_frame);
