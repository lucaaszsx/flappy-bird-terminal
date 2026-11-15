#include "game.h"
#include "window.h"
#include <bits/time.h>
#include <stdint.h>
#include <time.h>

#define NS_IN_SEC 1000000000ULL

// clear -> draw bg -> draw bird -> draw pipe

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Pipe;

const int width = 10;
const int height = 10;
Pipe pipe;
struct timespec frame_start, frame_end;
struct timespec last_pipe_frame = {0,0};

uint64_t delta_time2(struct timespec end, struct timespec start);

void game_init(win_t *win) {
    pipe.x = win->w - width;
    pipe.y = win->h - height;
    pipe.w = width;
    pipe.h = height;
}

void game_tick(win_t *win) {
    clock_gettime(CLOCK_MONOTONIC, &frame_start);

    win_draw_color(win, CL_GREEN);
    win_frect(win, 0, 0, win->w, win->h);

    win_draw_color(win, CL_YELLOW);
    win_frect(win, pipe.x, pipe.y, pipe.w, pipe.h);

    if (delta_time2(frame_start, last_pipe_frame) >= NS_IN_SEC / 2) {
        if (pipe.x <= 0) pipe.x = win->w - pipe.w;
        else pipe.x -= 1;
    }

    win_flush();
}

uint64_t delta_time2(struct timespec end, struct timespec start) {
    return (end.tv_sec - start.tv_sec) * NS_IN_SEC +
            (end.tv_nsec - start.tv_nsec);
}
