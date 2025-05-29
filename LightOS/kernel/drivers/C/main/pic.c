#include "pic.h"

// init PIC 8256A chip
// remap PIC vectors to avoid conflicts with CPU exceptions
// master offset is usually 0x20, slave offset 0x28
void pic_remap(const int offset_pic1, const int offset_pic2){
    uint32 a1, a2;
    //save masks
    a1 = in(PIC1_DATA);
    a2 = in(PIC2_DATA);

    // start initialization sequence (in cascade mode)  
    //here we send Initialization Control Word 1 (ICW1) to both PICs so that they enter initialization mode and become ready to receive the rest of the control words (ICW2–ICW4)

    //out (PIC1_COMMAND, 0x11);
    //send ICW1 (Initialization Command Word) to the master's "command" port (0x20).
    //the master PIC marks itself as "in initialization state" and waits for further commands

    //outb(PIC2_COMMAND, 0x11)
    //similarly: initialize the slave PIC (port 0xA0), also in cascade mode and with the requirement of ICW4                                icw1
    out(PIC1_COMMAND, 0x11);                          //first part of init. the controller will wait for other commands to fully initialize
    out(PIC2_COMMAND, 0x11);                          //this is how the hardware is designed


    // set vector offsets                              vector base in IVT = offset+IRQ      | 0x20, for example         icw2
    out(PIC1_DATA, offset_pic1);
    out(PIC2_DATA, offset_pic2);

    // tell Master PIC there is a slave at IRQ2 (0000 0100) |                                                           icw3
    out(PIC1_DATA, 0x04);   //to master: there is a slave on IRQ2 line
    // tell Slave PIC its cascade identity (0000 0010)
    out(PIC2_DATA, 0x02);   //slave: they are “hanging” on line #2 at the master

    // set 8086/88 (MCS-80/85) mode |                                                                                   icw4
    out(PIC1_DATA, 0x01);
    out(PIC2_DATA, 0x01);

    // restore saved masks
    out(PIC1_DATA, a1);
    out(PIC2_DATA, a2);
    return;
}
/*after you have remapped the PIC vector numbers, you need to create corresponding entries in your interrupt table (IDT) for these new vectors. 
otherwise, the processor will not know where to jump when it receives an IRQ signal.*/