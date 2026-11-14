#include "window.h"
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

void win_init(Window *win) {
    win_refresh(win);

    // enables alternate screen buffer and turns cursor invisible
    fprintf(stdout, "\033[?1049h\033[?25l");
    win_move_cursor(win, 0, 0);
    win_flush();
}

void win_stop() {
    // disable alternate screen buffer and turns cursor visible again
    fprintf(stdout, "\033[?1049l\x1b[?25h");
    win_flush();
}

void win_refresh(Window *win) {
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    win->w = ws.ws_col;
    win->h = ws.ws_row;
}

void win_move_cursor(Window *win, int x, int y) {
    // todo: verify if the x and y are in range of the width/height of the window
    // convert 0-indexed positions (x,y) to 1-indexed positions
    fprintf(stdout, "\033[%d;%dH", y + 1, x + 1);
}

void win_put_str(char *color, char *s) {
    if (color != NULL) fprintf(stdout, "%s%s" CL_NC, color, s);
    else fprintf(stdout, "%s", s);
}

void win_clear() {
    fprintf(stdout, "\033[H\033[2J\033[3J");
}

void win_frect(Window *win, int x, int y, int w, int h) {
    char rect_row[w + 1];
    memset(rect_row, 'X', w);
    rect_row[w] = '\0';

    for (int k = y; k < y + h; k++) {
        win_move_cursor(win, x, k);
        win_put_str(CL_GREEN, rect_row);
    }
}

void win_flush() {
    fflush(stdout);
}
