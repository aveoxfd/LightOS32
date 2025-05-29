#ifndef MEMORYMANIP_H
#define MEMORYMANIP_H

#include "portdrive.h"

void memset(void* destination, uint8 value, uint32 len);
void showdump(uint32 *address, uint32 len); //to do
#endif