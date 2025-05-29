
#ifndef VGA_H
#define VGA_H

#include "portdrive.h"

#define VGA_PORT_COLOR 0x3C8
#define VGA_WIDTH 320
#define VGA_HEIGHT 200
#define VGA_BUFFER ((uint8*)0xA0000)
#define BLACK 0x00
#define WHITE 0x01


extern const char font8x8_basic[128][8];
void putpixel(const unsigned int x, const unsigned int y, uint8 color);
void set_color(const uint32 table, const uint32 r, const uint32 g, const uint32 b);

#endif