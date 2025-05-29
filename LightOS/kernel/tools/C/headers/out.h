#ifndef PRINT_H
#define PRINT_H
#include "portdrive.h"
#include "vga.h"
void print_hex(const uint32, const uint32, const uint32);
void print_string(const uint32, const uint32, const char *, const uint32);
void draw_char(const uint32, const uint32, const char, uint32);
void clear_screen(const uint32 , const uint32 , uint32);
void clear_zone(const uint32 x, const uint32 y, const uint32 endx, const uint32 endy, const uint32 color);
#endif