#pragma once

typedef enum {
    CL_GREEN,
    CL_YELLOW,
    CL_NC // no color
} win_color_t;

typedef struct {
    unsigned short w;
    unsigned short h;
    win_color_t curr_color;
} win_t;

void win_init(win_t *win);
void win_stop();
void win_refresh(win_t *);
void win_move_cursor(win_t *, int x, int y);
void win_draw_color(win_t *win, win_color_t c);
void win_put_str(win_t *win, char *s);
void win_clear();
void win_puts(char *s);
void win_frect(win_t *, int x, int y, int w, int h);
void win_flush();
