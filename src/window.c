#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "window.h"
#include "util.h"

#define ANSI_RED    "\e[031m"
#define ANSI_GREEN  "\e[032m"
#define ANSI_YELLOW "\e[033m"
#define ANSI_BLUE   "\e[034m"
#define ANSI_RST    "\e[0m"

char *_get_cl_enum(win_color_t c);

void win_init(win_t *win) {
    win_refresh(win);
    win_block_char(win, "X");

    // default to uncolored output
    win_draw_color(win, CL_NC);

    // enables alternate screen buffer and turns cursor invisible
    fprintf(stdout, "\033[?1049h\033[?25l");
    win_move_cursor(0, 0);
    win_flush();
}

void win_stop(void) {
    // disable alternate screen buffer and turns cursor visible again
    fprintf(stdout, "\033[?1049l\x1b[?25h");
    win_flush();
}

void win_refresh(win_t *win) {
    struct winsize ws = {0};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0) {
        win_stop();
        perror("failed to get terminal window bounds: ");
        exit(1);
    }

    win->w = ws.ws_col;
    win->h = ws.ws_row;
}


void win_move_cursor(int x, int y) {
    // convert 0-indexed positions (x,y) to 1-indexed positions
    fprintf(stdout, "\033[%d;%dH", y + 1, x + 1);
}

void win_block_char(win_t *win, char *block_char) {
    win->block_char = block_char;
    win->block_len = strlen(block_char);
}

void win_draw_color(win_t *win, win_color_t c) {
    win->curr_color = c;
}

void win_put_str(win_t *win, char *s) {
    fprintf(stdout, "%s%s", _get_cl_enum(win->curr_color), s);
}

void win_clear(void) {
    fprintf(stdout, "\033[H\033[2J\033[3J");
}

void win_frect(win_t *win, int x, int y, int w, int h) {
    // verify horizontal bounds of the screen
    if (x + w >= win->w) {
        w = min(win->w - x, w);
    } else if (x < 0) {
        w = max(w + x, 0);
        x = 0;
    }

    // verify vertical bounds of the screen
    if (y >= win->h) {
        h = min(win->h - y, h);
    } else if (y < 0) {
        h = max(h + y, 0);
        y = 0;
    }

    // creates the entire row for the full rect layers
    // just a single time before draws
    size_t rr_size = w * win->block_len;
    char rect_row[rr_size + 1]; // +1 for the null-terminated string

    for (int k = 0; k < w; k++)
        memcpy(rect_row + k * win->block_len, win->block_char, win->block_len);

    rect_row[rr_size] = '\0';

    for (int k = y; k < y + h; k++) {
        win_move_cursor(x, k);
        win_put_str(win, rect_row);
    }
}

void win_flush(void) {
    fflush(stdout);
}

char *_get_cl_enum(win_color_t c) {
    switch (c) {
        case CL_RED:
            return ANSI_RED;
        case CL_GREEN:
            return ANSI_GREEN;
        case CL_YELLOW:
            return ANSI_YELLOW;
        case CL_BLUE:
            return ANSI_BLUE;
        default:
            return ANSI_RST;
    }
}
