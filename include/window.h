#pragma once

#define CL_GREEN "\e[032m"
#define CL_YELLOW "\e[033m"
#define CL_NC "\e[0m"

typedef struct {
    unsigned short w;
    unsigned short h;
} Window;

void win_init(Window *win);
void win_stop();
void win_refresh(Window *);
void win_move_cursor(Window *, int x, int y);
void win_clear();
void win_puts(char *s);
void win_frect(Window *, int x, int y, int w, int h);
void win_flush();
