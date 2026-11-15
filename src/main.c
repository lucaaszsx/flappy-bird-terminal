#include "window.h"
#include <bits/time.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define NS_IN_SEC 1000000000ULL

const float TARGET_FPS = 30.0f;
const uint64_t TARGET_FRAME_TIME = (uint64_t) (NS_IN_SEC / TARGET_FPS); // frames per nanosecond

win_t window;

void on_window_resize(int) {
    win_refresh(&window);
}

void on_exit_(int) {
    win_stop();
    exit(0);
}

int main() {
    signal(SIGWINCH, on_window_resize);
    signal(SIGINT, on_exit_);

    win_init(&window);

    struct timespec frame_start, frame_end;

    win_block_char(&window, "\u2588");

    while (1) {
        clock_gettime(CLOCK_MONOTONIC, &frame_start);

        // rendering
        win_clear();

        win_draw_color(&window, CL_YELLOW);
        win_frect(&window, 10, 10, 10, 10);
        win_draw_color(&window, CL_GREEN);
        win_frect(&window, 15, 5, 10, 10);

        win_flush();

        // fps limiter
        clock_gettime(CLOCK_MONOTONIC, &frame_end);

        uint64_t delta_time =
            (frame_end.tv_sec - frame_start.tv_sec) * NS_IN_SEC +
            (frame_end.tv_nsec - frame_start.tv_nsec);
        int64_t remaining = TARGET_FRAME_TIME - delta_time;
        if (remaining > 0) {
            struct timespec next_frame_ts = { remaining / NS_IN_SEC, remaining % NS_IN_SEC };
            nanosleep(&next_frame_ts, NULL);
        }
    }

    win_stop();
    return 0;
}
