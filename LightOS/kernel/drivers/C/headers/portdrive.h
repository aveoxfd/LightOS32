#ifndef LOWLEVEL_H
#define LOWLEVEL_H
typedef unsigned short  uint16;
typedef unsigned char   uint8;
typedef unsigned int    uint32;

extern uint16 in16(uint16);
extern void out16(uint16, uint16);

extern uint32 in32(uint32 port);
extern void out32(uint32 port, uint32 value);

extern uint8 in8(uint8 port);
extern void out8(uint8 port, uint8 value);

#define out out32
#define in in32

#endif