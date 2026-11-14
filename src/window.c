#include "window.h"
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#define ANSI_GREEN  "\e[032m"
#define ANSI_YELLOW "\e[033m"
#define ANSI_RST    "\e[0m"

char *_get_cl_enum(win_color_t c);

void win_init(win_t *win) {
    win_refresh(win);

    win->curr_color = CL_NC;

    // enables alternate screen buffer and turns cursor invisible
    fprintf(stdout, "\033[?1049h\033[?25l");
    win_move_cursor(win, 0, 0);
    win_flush();
}

void win_stop(void) {
    // disable alternate screen buffer and turns cursor visible again
    fprintf(stdout, "\033[?1049l\x1b[?25h");
    win_flush();
}

void win_refresh(win_t *win) {
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    win->w = ws.ws_col;
    win->h = ws.ws_row;
}

void win_move_cursor(win_t *win, int x, int y) {
    // todo: verify if the x and y are in range of the width/height of the window
    // convert 0-indexed positions (x,y) to 1-indexed positions
    fprintf(stdout, "\033[%d;%dH", y + 1, x + 1);
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
    char rect_row[w + 1];
    memset(rect_row, 'X', w);
    rect_row[w] = '\0';

    for (int k = y; k < y + h; k++) {
        win_move_cursor(win, x, k);
        win_put_str(win, rect_row);
    }
}

void win_flush(void) {
    fflush(stdout);
}

char *_get_cl_enum(win_color_t c) {
    switch (c) {
        case CL_GREEN:
            return ANSI_GREEN;
        case CL_YELLOW:
            return ANSI_YELLOW;
        default:
            return ANSI_RST;
    }
}
