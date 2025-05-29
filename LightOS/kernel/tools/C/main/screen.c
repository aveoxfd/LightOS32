
#include "out.h"
#include "screen.h"

void show_screen(Screen *scr){
    clear_screen(scr->width, scr->height, BLACK);
    for(int i = 0; i<scr->height; i++){
        for(int j = 0; j<scr->width; j++){
            if(scr->screen[i*scr->width+j]!=0) draw_char(j*CHAR_WIDTH, i*CHAR_HEIGHT, scr->screen[i*scr->width+j], WHITE);
        }
    }
    return;
}

void set_char(Screen *scr, const int x, const int y, const char c) {          ///++
    if (x >= 0 && x < scr->width && y >= 0 && y < scr->height) {
        scr->screen[y * scr->width + x] = c;
    }
    return;
}

void delete_char(Screen *scr, const int x, const int y){            //++
    if (x >= 0 && x < scr->width && y >= 0 && y < scr->height){
        scr->screen[y*scr->width+x] = 0;
    }
    return;
}

void copy_on_screen(Screen *scr, const int x, const int y, const char *source, const int len) {
    if (x < 0 || y < 0 || y >= scr->height) 
        return;
    
    int available = scr->width - x;
    int copy_len = (len > available) ? available : len;
    
    for (int i = 0; i < copy_len; i++) {
        scr->screen[y * scr->width + x + i] = source[i];
    }
}
char screen_buffer[SCREEN_CHAR_WIDTH * SCREEN_CHAR_HEIGHT];