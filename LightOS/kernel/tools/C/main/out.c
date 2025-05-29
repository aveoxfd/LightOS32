#include "out.h"

void print_string(const uint32 x, const uint32 y, const char *string, const uint32 color){
    for(int i = 0; string[i]!=0; i++){
        draw_char(x+8*i, y, string[i], color);
    }
    return;
}

void print_hex(const uint32 x, const uint32 y, const uint32 hex){
    char str[] = "0x00000000";
    uint32 mask = 0xF0000000;
    uint8 c = 0x1C;

    for(int i = 0; mask!=0; i++, mask>>=4, c-=4){
        uint8 x = ((hex & mask)>>c);
        if(x>9){
            x+=7;
        }
        x+='0';
        str[i+2]=x;
    }
    print_string(x, y, str, 0x01);
    return;
}

void draw_char(const uint32 x, const uint32 y, const char c, uint32 color){
    const char *chr = (uint8*)font8x8_basic[c];
    for (int i = 0; i<8; i++){
        uint8 bits = chr[i];
        for(int j = 0; j<8; j++){
            if(bits & (0b00000001<<j)){
                putpixel(x+j, y+i, color);
            }
        }
    }
    return;
}

void clear_screen(const uint32 x, const uint32 y, uint32 color){
    for(int i = 0; i<y; i++){
        for (int j = 0; j<x; j++){
            putpixel(j, i, color);
        }
    }
    return;
}

void clear_zone(const uint32 x, const uint32 y, const uint32 endx, const uint32 endy, const uint32 color){
    for(int i = y; i<endy; i++){
        for (int j = x; j<endx; j++){
            putpixel(j, i, color);
        }   
    }
    return;
}