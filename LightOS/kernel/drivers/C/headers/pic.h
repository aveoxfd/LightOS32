#ifndef PIC_h
#define PIC_H
#include "portdrive.h"

//ports for PIC 8259
#define PIC1_COMMAND 0x20   //controller PIC1 shadow command reg  
#define PIC1_DATA    0x21   //controller PIC1 IMR Mask reg
#define PIC2_COMMAND 0xA0   //
#define PIC2_DATA    0xA1   //
#define PIC_EOI      0x20   // End-of-interrupt command code

void pic_remap(const int, const int);
#endif

