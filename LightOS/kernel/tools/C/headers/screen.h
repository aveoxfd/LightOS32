
#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_PIXEL_WIDTH 320
#define SCREEN_PIXEL_HEIGHT 200

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

#define SCREEN_CHAR_WIDTH (SCREEN_PIXEL_WIDTH / CHAR_WIDTH)   // 40
#define SCREEN_CHAR_HEIGHT (SCREEN_PIXEL_HEIGHT / CHAR_HEIGHT) // 25

typedef struct Screen{
    int width, height;
    char *screen;
}Screen;

extern char screen_buffer[SCREEN_CHAR_WIDTH * SCREEN_CHAR_HEIGHT];

void show_screen(Screen *scr);
void set_char(Screen *scr, const int x, const int y,const char c);
void delete_char(Screen *scr, const int x, const int y);
void copy_on_screen(Screen *scr, const int x, const int y, const char * source, const int len);
#endif