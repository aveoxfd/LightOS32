[bits 32]
global keyboard_interrupt_handler
extern c_keyboard_handler
                            ;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                            ;when entering via 32-bit interrupt-gate IF (interrupt 0 - int disabled, 1 - enabled) is already reset, 
                            ;and when iret it will be restored automatically
keyboard_interrupt_handler:
pusha                       ;save all general purpose registers in stack

                            ;we can save segment registers if we want to access another GDT
call c_keyboard_handler     ;go to C func


mov al, 0x20
out 0x0A, al                ;EOI to slave        
out 0x20, al                ;EOI to master

popa                        ;load all general purpose registers from stack
iret                        ;load EFLAGS, CS, EIP from stack