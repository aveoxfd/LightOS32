#ifndef IDH_H
#define IDT_H

#include "portdrive.h"
#include "memory.h"
#include "pic.h"

//Interrupt Descriptor Table
//cpu do -> descriptor_address = IDTR.base + (vector_number × 8) in protected mode. This is a direct memory address (the presence of segments does not play a role in this formula)
typedef struct __attribute__((packed)) idt_descriptor{
    uint16  base_low;    //low address base
    uint16  selector;    //selector in GDT
    uint8    always0;    //Reserved: Must always be 0, otherwise the CPU may reject this descriptor.
    uint8      flags;    // 1 00   0     1110
                         // P DPL  S     Type - 32-bit interrupt gate
                         //
                         //P - descriptor "present"
                         //DPL - privilege level
                         //S - (S=0) is a system (non-segment) descriptor
                         //type - (Type=14) — the actual gateway type
    uint16 base_high;    //high address base
}idt_descriptor;                                                       // |base high|  flags  |  always0  |  selector  |  base low  |

typedef struct ptr_idt{
    uint16 limit;
    uint32 base;
}__attribute__((packed)) ptr_idt; //__attribute__((packed)) - do not align the structure 

#define IDT_ENTRIES 256

extern void void_handler();

#define cpu_exception_divide_error_handler                      void_handler
#define cpu_exception_debug_exception_handler                   void_handler
#define cpu_exception_nmi_handler                               void_handler
#define cpu_exception_breakpoint_handler                        void_handler
#define cpu_exception_overflow_handler                          void_handler
#define cpu_exception_bound_range_exceeded_handler              void_handler
#define cpu_exception_invalid_opcode_handler                    void_handler
#define cpu_exception_device_not_available_handler              void_handler
#define cpu_exception_double_fault_handler                      void_handler
#define cpu_exception_coprocessor_segment_overrun_handler       void_handler
#define cpu_exception_invalid_tss_handler                       void_handler
#define cpu_exception_segment_not_present_handler               void_handler
#define cpu_exception_stack_segment_fault_handler               void_handler
#define cpu_exception_general_protection_fault_handler          void_handler
#define cpu_exception_page_fault_handler                        void_handler
#define cpu_exception_reserved15_handler                        void_handler
#define cpu_exception_x87_floating_point_handler                void_handler
#define cpu_exception_alignment_check_handler                   void_handler
#define cpu_exception_machine_check_handler                     void_handler
#define cpu_exception_simd_floating_point_handler               void_handler
#define cpu_exception_virtualization_exception_handler          void_handler
#define cpu_exception_reserved21_handler                        void_handler
#define cpu_exception_reserved22_handler                        void_handler
#define cpu_exception_reserved23_handler                        void_handler
#define cpu_exception_reserved24_handler                        void_handler
#define cpu_exception_reserved25_handler                        void_handler
#define cpu_exception_reserved26_handler                        void_handler
#define cpu_exception_reserved27_handler                        void_handler
#define cpu_exception_reserved28_handler                        void_handler
#define cpu_exception_reserved29_handler                        void_handler
#define cpu_exception_reserved30_handler                        void_handler
#define cpu_exception_reserved31_handler                        void_handler



extern void cpu_exception_divide_error_handler();               // Vector 0x00
extern void cpu_exception_debug_exception_handler();            // Vector 0x01
extern void cpu_exception_nmi_handler();                        // Vector 0x02
extern void cpu_exception_breakpoint_handler();                 // Vector 0x03
extern void cpu_exception_overflow_handler();                   // Vector 0x04
extern void cpu_exception_bound_range_exceeded_handler();       // Vector 0x05
extern void cpu_exception_invalid_opcode_handler();             // Vector 0x06
extern void cpu_exception_device_not_available_handler();       // Vector 0x07
extern void cpu_exception_double_fault_handler();               // Vector 0x08
extern void cpu_exception_coprocessor_segment_overrun_handler();// Vector 0x09
extern void cpu_exception_invalid_tss_handler();                // Vector 0x0A
extern void cpu_exception_segment_not_present_handler();        // Vector 0x0B
extern void cpu_exception_stack_segment_fault_handler();        // Vector 0x0C
extern void cpu_exception_general_protection_fault_handler();   // Vector 0x0D
extern void cpu_exception_page_fault_handler();                 // Vector 0x0E
extern void cpu_exception_reserved15_handler();                 // Vector 0x0F
extern void cpu_exception_x87_floating_point_handler();         // Vector 0x10
extern void cpu_exception_alignment_check_handler();            // Vector 0x11
extern void cpu_exception_machine_check_handler();              // Vector 0x12
extern void cpu_exception_simd_floating_point_handler();        // Vector 0x13
extern void cpu_exception_virtualization_exception_handler();   // Vector 0x14
extern void cpu_exception_reserved21_handler();                 // Vector 0x15
extern void cpu_exception_reserved22_handler();                 // Vector 0x16
extern void cpu_exception_reserved23_handler();                 // Vector 0x17
extern void cpu_exception_reserved24_handler();                 // Vector 0x18
extern void cpu_exception_reserved25_handler();                 // Vector 0x19
extern void cpu_exception_reserved26_handler();                 // Vector 0x1A
extern void cpu_exception_reserved27_handler();                 // Vector 0x1B
extern void cpu_exception_reserved28_handler();                 // Vector 0x1C
extern void cpu_exception_reserved29_handler();                 // Vector 0x1D
extern void cpu_exception_reserved30_handler();                 // Vector 0x1E
extern void cpu_exception_reserved31_handler();                 // Vector 0x1F


extern void timer_interrupt_handler();                           //Vector 0x20
extern void keyboard_interrupt_handler();                        //Vector 0x21


void IDTinit(void);
void idt_set_descriptor(idt_descriptor *descriptor, const uint32 base, const uint16 selector, const uint8 flags);
#endif
/*
The IDT in protected mode can be located anywhere and is loaded via IDTR:
When you switch to protected mode, you programmatically specify the address of the new IDT (the lidt command). 
This allows you to place it in any free memory area - usually at the beginning of your kernel or just after the final code segment. 
Placing the IDT right after the IVT is pointless and dangerous, because that area is occupied by the BIOS, and you have no guarantee that it is free and that you will not damage something important.

IVT in real-mode is strictly fixed at address 0:
At the very beginning of physical memory (from 0x00000 to 0x003FF) is the IVT - 256 entries of 4 bytes each. The processor in real-mode strictly looks only there, and its location cannot be changed in any way.

The BIOS Data Area and other structures immediately follow the IVT:
The so-called "BIOS Data Area" (BDA) begins at address 0x0400, where the BIOS stores its tables (keyboard settings, video mode, hardware, etc.). 
If you put your IDT there at the same time, you will simply erase dozens of important bytes, and the system will hang.
*/