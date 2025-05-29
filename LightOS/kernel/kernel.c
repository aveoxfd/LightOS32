//__asm__(".code16gcc\n");
    #include "drivers.h"
    #include "interrupts.h"
    #include "tools.h"
__asm__(
    ".org 0x0000\n"
    ".byte 0x01, 0x01\n"
    "jmp kernel"
); 
void kernel(void) {      // __attribute__((naked)) - no prologue 
    IDTinit();
    __asm__("sti");
    set_color(BLACK, 0, 0, 0);
    set_color(WHITE, 255,255,255);

    Screen main;
    main.height = SCREEN_PIXEL_HEIGHT;
    main.width = SCREEN_PIXEL_WIDTH;
    main.screen = screen_buffer;

    copy_on_screen(&main,0, 0, "Hello!", 7);
    copy_on_screen(&main, 0, 1, ":/ ", 2);
    while(1){
        show_screen(&main);
        for(int i = 0; i!=256; i++)copy_on_screen(&main, 2+i, 1, &keyboard_buffer[i], 1);
    }
}