#include "portdrive.h"
#include "vga.h"
#include "tools.h"


static const char keymap[128] = {
    /* 0x00 */ 0,    
    /* 0x01 */ 0,    // ESC
    /* 0x02 */ '1', 
    /* 0x03 */ '2', 
    /* 0x04 */ '3', 
    /* 0x05 */ '4', 
    /* 0x06 */ '5', 
    /* 0x07 */ '6', 
    /* 0x08 */ '7', 
    /* 0x09 */ '8', 
    /* 0x0A */ '9', 
    /* 0x0B */ '0', 
    /* 0x0C */ '-', 
    /* 0x0D */ '=', 
    /* 0x0E */ '\b', // Backspace
    /* 0x0F */ '\t', // Tab
    /* 0x10 */ 'q', 
    /* 0x11 */ 'w', 
    /* 0x12 */ 'e', 
    /* 0x13 */ 'r', 
    /* 0x14 */ 't', 
    /* 0x15 */ 'y', 
    /* 0x16 */ 'u', 
    /* 0x17 */ 'i', 
    /* 0x18 */ 'o', 
    /* 0x19 */ 'p', 
    /* 0x1A */ '[', 
    /* 0x1B */ ']', 
    /* 0x1C */ '\n', // Enter
    /* 0x1D */ 0,    // Left Control
    /* 0x1E */ 'a', 
    /* 0x1F */ 's', 
    /* 0x20 */ 'd', 
    /* 0x21 */ 'f', 
    /* 0x22 */ 'g', 
    /* 0x23 */ 'h', 
    /* 0x24 */ 'j', 
    /* 0x25 */ 'k', 
    /* 0x26 */ 'l', 
    /* 0x27 */ ';', 
    /* 0x28 */ '\'', 
    /* 0x29 */ '`', 
    /* 0x2A */ 0,    // Left Shift
    /* 0x2B */ '\\', 
    /* 0x2C */ 'z', 
    /* 0x2D */ 'x', 
    /* 0x2E */ 'c', 
    /* 0x2F */ 'v', 
    /* 0x30 */ 'b', 
    /* 0x31 */ 'n', 
    /* 0x32 */ 'm', 
    /* 0x33 */ ',', 
    /* 0x34 */ '.', 
    /* 0x35 */ '/', 
    /* 0x36 */ 0,    // Right Shift
    /* 0x37 */ '*',  // Numpad *
    /* 0x38 */ 0,    // Alt
    /* 0x39 */ ' ',  // Space
    /* 0x3A */ 0,    // Caps Lock
    /* 0x3B */ 0,    // F1
    /* 0x3C */ 0,    // F2
};

// Таблица для Shift-клавиш
static const char shift_keymap[128] = {
    /* 0x00 */ 0,
    /* 0x01 */ 0,
    /* 0x02 */ '!', 
    /* 0x03 */ '@', 
    /* 0x04 */ '#', 
    /* 0x05 */ '$', 
    /* 0x06 */ '%', 
    /* 0x07 */ '^', 
    /* 0x08 */ '&', 
    /* 0x09 */ '*', 
    /* 0x0A */ '(', 
    /* 0x0B */ ')', 
    /* 0x0C */ '_', 
    /* 0x0D */ '+', 
    /* 0x0E */ '\b',
    /* 0x0F */ '\t',
    /* 0x10 */ 'Q', 
    /* 0x11 */ 'W', 
    /* 0x12 */ 'E', 
    /* 0x13 */ 'R', 
    /* 0x14 */ 'T', 
    /* 0x15 */ 'Y', 
    /* 0x16 */ 'U', 
    /* 0x17 */ 'I', 
    /* 0x18 */ 'O', 
    /* 0x19 */ 'P', 
    /* 0x1A */ '{', 
    /* 0x1B */ '}', 
    /* 0x1C */ '\n',
    /* 0x1D */ 0,
    /* 0x1E */ 'A', 
    /* 0x1F */ 'S', 
    /* 0x20 */ 'D', 
    /* 0x21 */ 'F', 
    /* 0x22 */ 'G', 
    /* 0x23 */ 'H', 
    /* 0x24 */ 'J', 
    /* 0x25 */ 'K', 
    /* 0x26 */ 'L', 
    /* 0x27 */ ':', 
    /* 0x28 */ '"', 
    /* 0x29 */ '~', 
    /* 0x2A */ 0,
    /* 0x2B */ '|', 
    /* 0x2C */ 'Z', 
    /* 0x2D */ 'X', 
    /* 0x2E */ 'C', 
    /* 0x2F */ 'V', 
    /* 0x30 */ 'B', 
    /* 0x31 */ 'N', 
    /* 0x32 */ 'M', 
    /* 0x33 */ '<', 
    /* 0x34 */ '>', 
    /* 0x35 */ '?', 
    /* 0x36 */ 0,
    /* 0x37 */ '*',
    /* 0x38 */ 0,
    /* 0x39 */ ' ',
    
};

struct keyboard_status{
    uint8 shft_p : 1;
    uint8 cps_l  : 1;
}keyboard_status;

void c_keyboard_handler(void){
    struct keyboard_status kb;
    uint8 code = in(0x60);
    if (code & 0x80){
        uint8 released = code & 0x7F;
        if (released == 0x2A || released == 0X36) kb.shft_p = 0;
        return;
    }
    switch(code){
        case 0x2A:
        case 0x36:
        kb.shft_p=1;
        break;
        case 0x3A:
        kb.cps_l = !kb.cps_l;
        break;
        case 0x0E:
        if(keyboard_buffer_ptr>0){
            remove_from_buffer();
        }
        break;
        default:
        if(code<sizeof(keymap)){
            uint8 ascii = 0;

            if(kb.shft_p){
                ascii = shift_keymap[code];
            }
            else{
                ascii = keymap[code];
            }
            if (ascii){
                if(kb.cps_l && ascii >= 'a' && ascii <= 'z'){
                    ascii -=32;
                }
                else if(kb.cps_l && ascii >= 'A' && ascii <= 'Z'){
                    ascii+=32;
                }
                add_to_buffer(ascii);
            }
        }
    }
    return;
}