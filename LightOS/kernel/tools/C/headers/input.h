#ifndef INPUT_H
#define INPUT_H
#define KEYBOARD_BUFFER_SIZE 256

extern char keyboard_buffer[256];
extern unsigned char keyboard_buffer_ptr;
void add_to_buffer(unsigned char);
void remove_from_buffer(void);
#endif