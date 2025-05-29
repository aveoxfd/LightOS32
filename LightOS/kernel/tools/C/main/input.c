#include "input.h"

char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
unsigned char keyboard_buffer_ptr = 0;

void add_to_buffer(unsigned char c){
    keyboard_buffer[keyboard_buffer_ptr]=c;
    keyboard_buffer_ptr++;
    return;
}
void remove_from_buffer(void){
    if(keyboard_buffer_ptr>=0){
        keyboard_buffer[keyboard_buffer_ptr]=0;
        keyboard_buffer_ptr--;
    }
    return;
}