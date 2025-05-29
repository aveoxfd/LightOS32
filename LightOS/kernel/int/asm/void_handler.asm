
[bits 32]
global void_handler
                    ;when entering via 32-bit interrupt-gate IF (interrupt flag 0 - int disabled, 1 - enabled) is already reset, 
                    ;and when iret it will be restored automatically
void_handler:       ;when the processor moves through an interrupt vector, it saves the state of the previous work (EFLAGS, CS, EIP) to the stack
pusha               ;save all general purpose registers

mov eax, 0x20
out 0x0A, al        ;EOI to slave    
out 0x20, al        ;EOI to master

popa
iret                ;load EFLAGS, CS, EIP from stack