#pragma once

#include "window.h"
#include <stddef.h>
#include <time.h>

#define TOTAL_PIPES 3

typedef struct {
    size_t x;
    size_t y;
    size_t w;
    size_t h;
} pipe_t;

typedef struct {
    win_t *win;
    pipe_t pipes[TOTAL_PIPES];
} game_t;

void game_init(win_t *win);
void game_refresh(win_t last_win, win_t *current_win);
void game_tick(win_t *win, struct timespec current_frame);
