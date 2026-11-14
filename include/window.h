#pragma once

#define CL_GREEN "\e[032m"
#define CL_YELLOW "\e[033m"
#define CL_NC "\e[0m"

typedef struct {
    unsigned short w;
    unsigned short h;
} win_t;

void win_init(win_t *win);
void win_stop();
void win_refresh(win_t *);
void win_move_cursor(win_t *, int x, int y);
void win_clear();
void win_puts(char *s);
void win_frect(win_t *, int x, int y, int w, int h);
void win_flush();
