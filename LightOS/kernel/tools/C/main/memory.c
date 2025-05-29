#include "memory.h"
#include "portdrive.h"

void memset(void* destination, uint8 value, uint32 len){
    uint8* ptr = (uint8*)destination;
    for (uint32 i = 0; i<len; i++){
        ptr[i] = value;
    }
    return;
}
