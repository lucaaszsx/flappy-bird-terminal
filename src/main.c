#include "window.h"
#include "game.h"
#include "util.h"
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

const float TARGET_FPS = 30.0f;
const uint64_t TARGET_FRAME_TIME = (uint64_t) (NS_IN_SEC / TARGET_FPS); // frames per nanosecond

win_t window;

void handle_window_resize(int);
void handle_exit(int);

int main() {
    signal(SIGWINCH, handle_window_resize);
    signal(SIGINT, handle_exit);

    win_init(&window);
    game_init(&window);

    struct timespec frame_start, frame_end;

    win_block_char(&window, "\u2588");

    while (1) {
        clock_gettime(CLOCK_MONOTONIC, &frame_start);

        // rendering
        game_tick(&window, frame_start);

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

void handle_window_resize(int _) {
    win_t last_win = window;

    win_refresh(&window);
    game_refresh(last_win, &window);
}

void handle_exit(int _) {
    win_stop();
    exit(0);
}
