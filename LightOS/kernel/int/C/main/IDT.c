/*
* In Protected Mode, the CPU generates a divide-by-zero exception as vector 0x00.
* The descriptor address is calculated using the formula:
*
* descriptor_addr = IDTR.base + vector * sizeof(idt_descriptor)
*
* So for vector 0x00, the CPU will always take the IDT[0] entry.
* If we had put a keyboard handler there, any divide-by-zero
* would have resulted in this handler being called.
*
* Also, by default, the first (master) PIC is configured for base vector 0x08,
* so hardware IRQ0…IRQ7 send INT 0x08…0x0F. Since the keyboard is IRQ1,
* without remapping, it will output as INT 0x09 (9), and the CPU will go to IDT[9],
* and not your IDT[0].
*
* Therefore we:
* 1) Load our IDT (lidt), so that the CPU looks in our table, and not in the BIOS-IVT.
* 2) Remap the PIC with offset 0x20/0x28, so that the hardware IRQ lines go to vectors 32…47.
* 3) Hang the keyboard on IDT[33] (vector 0x21), where the PIC now sends IRQ1.
*/
#include "IDT.h"

idt_descriptor IDT[IDT_ENTRIES];
ptr_idt IDTPTR;

void idt_set_descriptor(idt_descriptor *descriptor, const uint32 base, const uint16 selector, const uint8 flags){
    descriptor->base_low = (base & 0x0000FFFF);
    descriptor->selector = selector;
    descriptor->always0  = 0x0000;
    descriptor->flags    = flags;
    descriptor->base_high = (base & 0xFFFF0000); //main code base       base high | base low = 32 bit address
    return;                                      
}
void IDTinit(){
    memset(&IDT, 0, sizeof(IDT));

    IDTPTR.limit = sizeof(IDT) - 1; //256*8 = 2047
    IDTPTR.base = (uint32)&IDT;
    __asm__ volatile("lidt (%0)" : : "r"(&IDTPTR)); //load in interrupt descriptor table register | base            | limit |

    pic_remap(0x20, 0x28);          //because cpu have 32 hardware interrupt (exceptions)

    for (int i = 0; i<IDT_ENTRIES; i++){
        idt_set_descriptor(&IDT[i], (uint32)void_handler, 0x08, 0x8E);
    }
    idt_set_descriptor(&IDT[0x00], (uint32)cpu_exception_divide_error_handler,                              0x08, 0x8E);
    idt_set_descriptor(&IDT[0x01], (uint32)cpu_exception_debug_exception_handler,                           0x08, 0x8E);
    idt_set_descriptor(&IDT[0x02], (uint32)cpu_exception_nmi_handler,                                       0x08, 0x8E);
    idt_set_descriptor(&IDT[0x03], (uint32)cpu_exception_breakpoint_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x04], (uint32)cpu_exception_overflow_handler,                                  0x08, 0x8E);
    idt_set_descriptor(&IDT[0x05], (uint32)cpu_exception_bound_range_exceeded_handler,                      0x08, 0x8E);
    idt_set_descriptor(&IDT[0x06], (uint32)cpu_exception_invalid_opcode_handler,                            0x08, 0x8E);
    idt_set_descriptor(&IDT[0x07], (uint32)cpu_exception_device_not_available_handler,                      0x08, 0x8E);
    idt_set_descriptor(&IDT[0x08], (uint32)cpu_exception_double_fault_handler,                              0x08, 0x8E);
    idt_set_descriptor(&IDT[0x09], (uint32)cpu_exception_coprocessor_segment_overrun_handler,               0x08, 0x8E);
    idt_set_descriptor(&IDT[0x0A], (uint32)cpu_exception_invalid_tss_handler,                               0x08, 0x8E);
    idt_set_descriptor(&IDT[0x0B], (uint32)cpu_exception_segment_not_present_handler,                       0x08, 0x8E);
    idt_set_descriptor(&IDT[0x0C], (uint32)cpu_exception_stack_segment_fault_handler,                       0x08, 0x8E);
    idt_set_descriptor(&IDT[0x0D], (uint32)cpu_exception_general_protection_fault_handler,                  0x08, 0x8E);
    idt_set_descriptor(&IDT[0x0E], (uint32)cpu_exception_page_fault_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x0F], (uint32)cpu_exception_reserved15_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x10], (uint32)cpu_exception_x87_floating_point_handler,                        0x08, 0x8E);
    idt_set_descriptor(&IDT[0x11], (uint32)cpu_exception_alignment_check_handler,                           0x08, 0x8E);
    idt_set_descriptor(&IDT[0x12], (uint32)cpu_exception_machine_check_handler,                             0x08, 0x8E);
    idt_set_descriptor(&IDT[0x13], (uint32)cpu_exception_simd_floating_point_handler,                       0x08, 0x8E);
    idt_set_descriptor(&IDT[0x14], (uint32)cpu_exception_virtualization_exception_handler,                  0x08, 0x8E);
    idt_set_descriptor(&IDT[0x15], (uint32)cpu_exception_reserved21_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x16], (uint32)cpu_exception_reserved22_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x17], (uint32)cpu_exception_reserved23_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x18], (uint32)cpu_exception_reserved24_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x19], (uint32)cpu_exception_reserved25_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x1A], (uint32)cpu_exception_reserved26_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x1B], (uint32)cpu_exception_reserved27_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x1C], (uint32)cpu_exception_reserved28_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x1D], (uint32)cpu_exception_reserved29_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x1E], (uint32)cpu_exception_reserved30_handler,                                0x08, 0x8E);
    idt_set_descriptor(&IDT[0x1F], (uint32)cpu_exception_reserved31_handler,                                0x08, 0x8E);

    idt_set_descriptor(&IDT[0x20], (uint32)timer_interrupt_handler,                                         0x08, 0x8E);
    idt_set_descriptor(&IDT[0x21], (uint32)keyboard_interrupt_handler,                                      0x08, 0x8E);

    //out(PIC1_DATA, 0xFC);  //only timer & keyboard        11111100
    //out(PIC2_DATA, 0xFF);  //full block all interrupts    11111111
    return;
}

/*After the CPU has calculated the address of the descriptor in the IDT and followed it, something like this sequence of actions occurs:

Reading the descriptor from memory
The CPU does:

descriptor_addr = IDTR.base + vector_number * 8
and reads 8 bytes of your idt_entry structure (what is called idt_descriptor in C code) from memory.

Parsing descriptor fields
From these 8 bytes it extracts:

base_low and base_high → full 32-bit handler address (handler_addr = base_lo | (base_hi << 16))

sel -> selector of code segment to switch to (usually 0x08)

flags -> checks P bit (descriptor present), checks access rights (DPL)

Saving context
Before switching CS:EIP, the processor automatically does:

push EFLAGS ; saves flags (including old IF)
push CS ; saves old text segment
push EIP ; saves old program counter
At the same time, since this is a 32-bit Interrupt Gate (type=14), the CPU clears the IF flag, disabling nested hardware IRQs until you explicitly call sti.

Switching to handler
After this, the CPU loads:

CS = sel
EIP = handler_addr
and starts executing the code at the new address, getting into your asm-stub (for example, irq1_stub for the keyboard).*/