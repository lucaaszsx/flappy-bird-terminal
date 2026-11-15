#include "window.h"
#include "game.h"
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define NS_IN_SEC 1000000000ULL

const float TARGET_FPS = 30.0f;
const uint64_t TARGET_FRAME_TIME = (uint64_t) (NS_IN_SEC / TARGET_FPS); // frames per nanosecond

win_t window;

uint64_t delta_time(struct timespec end, struct timespec start);
void on_window_resize(int);
void on_exit_(int);

int main() {
    signal(SIGWINCH, on_window_resize);
    signal(SIGINT, on_exit_);

    win_init(&window);
    game_init(&window);

    struct timespec frame_start, frame_end;

    win_block_char(&window, "\u2588");

    win_color_t c1 = CL_YELLOW, c2 = CL_GREEN;

    while (1) {
        clock_gettime(CLOCK_MONOTONIC, &frame_start);

        // rendering
        game_tick(&window);

        // fps limiter
        clock_gettime(CLOCK_MONOTONIC, &frame_end);

        int64_t remaining = TARGET_FRAME_TIME - delta_time(frame_end, frame_start);
        if (remaining > 0) {
            struct timespec next_frame_ts = { remaining / NS_IN_SEC, remaining % NS_IN_SEC };
            nanosleep(&next_frame_ts, NULL);
        }
    }

    win_stop();
    return 0;
}

uint64_t delta_time(struct timespec end, struct timespec start) {
    return (end.tv_sec - start.tv_sec) * NS_IN_SEC +
            (end.tv_nsec - start.tv_nsec);
}


void on_window_resize(int) {
    win_refresh(&window);
}

void on_exit_(int) {
    win_stop();
    exit(0);
}
