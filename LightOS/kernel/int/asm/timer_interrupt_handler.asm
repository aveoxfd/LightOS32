[bits 32]
global timer_interrupt_handler
timer_interrupt_handler:
pusha               ;save all general purpose registers

mov al, 0x20
out 0xa0, al        ;EOI to slave
out 0x20, al        ;EOI to master

popa                ;load all general purpose registers from stack
iret                ;load EFLAGS, CS, EIP from stack