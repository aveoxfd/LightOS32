KERNEL_START equ 0x8c24                          
KERNEL_PATTERN equ 0x0101

[org 0x7e00]                                        ;0x7c00+512(size Boot loader)       offset+[org]
PART_BOOTLOADER_START equ $                         ;0x7e00

  
disk_rw dw 0x0000                                   ;0x7e00     function from first part of bootloader
print_string_first_part dw 0x0000                   ;0x7e02     function from first part of bootloader
print_hex_word_first_part dw 0x0000                 ;0x7e04     function from first part of bootloader


;==================================================MAIN LOGIC=================================================
[bits 16] 
start:                                              ;0x7e06

push SECOND_PART_MESSAGE                            
call [print_string_first_part] 
call get_ip
get_ip:
pop ax
push ax
call [print_hex_word_first_part]  
push newstr 
call [print_string_first_part]

push word KERNEL_START                              ;load kernel in RAM on address
push word 0x0001                                    ;start from 1 sector
push word KERNEL_PATTERN                            ;personal pattern of kernel
call load_kernel

mov ax, 0x0013
int 0x10

cli                                                  ;turn off interrupts, 
                                                     ;because the interrupt table is configured for BIOS and it is not compatible with our operating mode, 
                                                     ;since the BIOS has its own address transitions, and we now have selectors GDT, not segments

call switch_to_pm

[bits 32]
;db 0x66                                             ;funny <------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   Got to ***1***
;                                                    ;this happens because until the FAR-JMP your CS.D is still 0 (real-mode 16-bit segment), and only the FAR-JMP itself loads a new selector with D=1
;                                                    ;the processor pre-loads instruction bytes into the prefetch queue BEFORE you switch to Protected Mode.
;                                                    ;before jmp far, you are actually working on “preloaded” real-mode bytes, and limit checks for code already in the queue do not occur.
;                                                    ;jmp far itself is executed from this queue, switches CS → new limit (usually 4 GB) and only then starts taking further commands according to Protected Mode rules.
;                                                    ;therefore, the “zero” limit does not actually have time to cut you off
;                                                    ;i highly recommend learning about the computational pipeline   !!!!!!
;
;jmp CODE_SEGMENT:protected_mode_entry               ;     jmp    CODE_SEGMENT:protected_mode_entry   |  far jump, reset pipeline. code segment will be changed by cpu
;                                                    ;0x66 0xEA    protected_mode_entry CODE_SEGMENT  |       
;
;
call KERNEL_START                                    ;now we going in kernel
;==================================================================================================================================

;==================================================================================================================================
print_hex_dw:                                        ;print_hex_dw(dword hex)
push ebp        
mov ebp, esp        
sub esp, 11                                          ;reserve space for text

mov eax, esp                                         ;->0xFFFFFFFF
mov byte [eax], '0'     
mov byte [eax+1], 'x'       
mov byte [eax+10], 0        
add eax, 2      
mov [ebp-11-4], eax                                  ;*buffer

mov eax, 0xF0000000                                  ;mask
mov [ebp-11-8], eax                                  ;save mask

mov eax, 0x0000001C                                  ;28
mov [ebp-11-12], eax                                 ;save 28

.loop:      

mov eax, [ebp-11-8]     
cmp eax, 0      
je .exit        

mov ebx, [ebp+8]                                     ;load hex
and ebx, eax                                         ;hex 0xFFFFFFFF & 0xF0000000
mov ecx, [ebp-11-12]
shr ebx, cl

shr eax, 4
mov [ebp-11-8], eax
mov eax, [ebp-11-12]
sub eax, 4
mov [ebp-11-12], eax

cmp bl, 9
jle .digit
add bl, 7

.digit:
add bl, '0'

mov eax, [ebp-11-4]
mov [eax], bl
inc eax
mov [ebp-11-4], eax

jmp .loop

.exit:
mov eax, ebp
sub eax, 11
push eax
call print_string
add esp, 11                                          ;free space
mov esp, ebp
pop ebp
ret
;===========================================

;===========================================
print_string:                                       ;print_string(*string)
push ebp        
mov ebp, esp        

xor eax, eax                                        ;save pointer
mov eax, [ebp+8]        
mov [ebp-4], eax        

mov eax, 0xB8000                                    ;*video memory
mov [ebp-8], eax                                    ;save *video memory

.loop:      

xor eax, eax        
mov eax, [ebp-4]        
mov al, [eax]                                       ;load char

cmp al, 0       
je .exit        

mov ah, 0x07                                        ;attribute

mov ebx, [ebp-8]
mov [ebx], ax
add ebx, 2
mov [ebp-8], ebx

mov eax, [ebp-4]
inc eax
mov [ebp-4], eax

jmp .loop

.exit:
mov esp, ebp
pop ebp
ret
;============================================


[bits 16]
;============================================
load_kernel:                                 ; void load_kernel(address_in_RAM, start_sector, pattern)
push bp                                      ;save base pointer  sp++
mov bp, sp                                   ;base pointer = stack pointer  

sub sp, 2                                    ;protect local data
mov ax, [bp+6]                               ;load from argument stack sector   
mov [bp-2], ax                               ;save it in local data in func

.find:                                       ;while
mov ax, [bp-2]                               ;load from loacal data
push ax                                      ;
call [print_hex_word_first_part]
push SPACE
call [print_string_first_part]

push 0x0000                                   ;segment
push 0x0020                                   ;read one sector
mov ax, [bp+8]                                ;load address from argument stack sector
push ax                                       ;push it
push 0x0000                                   ;cylinder
push 0x0000                                   ;head
mov ax, [bp-2]                                ;load saved readed sectors from local sector
push ax                                       ;push it
push 0x0080                                   ;device (hdd)
call [disk_rw]

mov si, [bp+8]                                ;load address from argument sector in stack
mov word ax, [si]                             ;indirect addressing in RAM -> ax
push ax                                       ;push it (to print)
call [print_hex_word_first_part]              ;
push SPACE                                    ;
call [print_string_first_part]                ;
mov si, [bp+8]                                ;indirect addressing in RAM -> si | this needs to be repeated 
mov word ax, [si]                             ;because the previous BIOS calls and text renderings have ruined ah and si
mov bx, [bp+4]                                ;indirect addressing in RAM -> bx
cmp ax, bx                                    ;if ax==bx -> pattern was founded

je .founded                                   ;if +
jmp .continue                                 ;else

.founded:
push FIND                                     ;==print==
call [print_string_first_part]
mov ax, [bp+4]                                ;print pattern
push ax
call [print_hex_word_first_part]
push FIND_CONTINUE
call [print_string_first_part]
mov ax, [bp-2]                                ;print sector
push ax
call [print_hex_word_first_part]              ;========
jmp .exit

.continue:                                    ;else block
mov ax, [bp-2]                                ;local data sector++
inc ax
mov [bp-2], ax

jmp .find                                     ;while 

.exit:                                        ;
add sp, 2                                     ;alloc sp so as not to disturb the balance of the stack
mov sp, bp                                    ;stack pointer = base pointer
pop bp                                        ;base pointer <- stack
ret                                           ;return
SECTOR_READ_ERROR db "Sector read error ", 0
FIND_SUCCESS:
FIND db "Find " , 0x00
FIND_CONTINUE db " pattern in sector ", 0
;============================================

;============================================
switch_to_pm:

lgdt [GDT_DESCRIPTOR_REGISTER]                      ; load gdt
mov eax, cr0                                        ;we need the least significant bit of the CR0 register (PE - Protect Enable)
or eax, 1                                           ;set last bit 1
mov cr0, eax                                        ;update CR 0

;jmp CODE_SEGMENT:protected_mode_entry              ;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

[bits 32]                                           ;***1***!!! So import
                                                    ;Context switching is required for the compiler and for the processor, 
                                                    ;because the processor, working in protected mode, 
                                                    ;requires more bytes if the command does not specify a prefix for changing the size. 
                                                    ;Although the commands are the same (mostly), 
                                                    ;but in different modes they require different sizes and in general the context changes
db 0x66                                             ;if you write a jump command after changing the compiler context, this command will be translated as 32-bit, 
                                                    ;which is correct, but the CPU “preloads” the instruction bytes into the prefetch queue BEFORE you switch to Protected Mode. The instructions 
                                                    ;mov cr0, eax
                                                    ;db 0x66
                                                    ;jmp far …
                                                    ;have already been completely in this queue and are executed without re-checking the CS/limit. ar-jmp will flush the queue
                                                    ;– As soon as jmp far is encountered, the CPU flushes the prefetch queue and loads a new CS from the GDT (with selector 0x08, which has a normal limit).
                                                    ;– From this point on, all new instruction fetches and limit checks are already done by the correct descriptor.
                                                    ;
                                                    ;Before jmp far, you are actually working on “preloaded” real-mode bytes, and limit checks for the code already in the queue do not occur.
                                                    ;– The jmp far itself is executed from this queue, switches CS → new limit (usually 4 GB) and only then starts taking commands further according to the Protected Mode rules.
                                                    ;– Therefore, the “zero” limit actually does not have time to cut you off.
jmp CODE_SEGMENT:.protected_mode_entry               ;reset pipeline, update selector (cs)

.protected_mode_entry:                               ;update segments registers
mov ax, DATA_SEGMENT		
mov ds, ax							
mov es, ax
mov fs, ax
mov ss, ax	
mov gs, ax

mov eax, 0x0FFFFFFF                               ;realloc the stack if needed
mov esp, eax                       
mov ebp, eax	

                                                  ;the stack can be left unchanged if you are not going to reinstall it, since it is not changed by hardware

jmp KERNEL_START                                  ;go to kernel
;============================================

GDT:
NULL_DESCRIPTOR:                                                
db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
GDT_CODE_KERNEL_SEGMENT:                                       ;code segment
    dw 0xFFFF       ; limit 15:0 (0xFFFF + 0xf below = 0xFFFF)
    dw 0x0000       ; base 31:16 (0x0000)   little
    db 0x00         ; base 23:16 (0x00)     medium
    db 0b10011010   ; access byte:
                    ; p: 0 = not present (fault) | 1=present
                    ; dpl: 00 = ring0 | 01 = ring1 | 10 = ring2 | 11 = ring3
                    ; s: 0 = system | 1 = code/data
                    ; type = 1010:
                    ;   code/data: 0 = data | 1 = code
                    ;   conforming: 0 = non-conforming | 1 = conforming
                    ;   readable: 0 = execute-only | 1 = readable
                    ;   accessed: 0 = not accessed | 1 = accessed (auto-set by cpu)
    db 0b11001111   ; flags:
                    ; g: 0 = bytes | 1 = 4kb pages
                    ; d/b: 0 = 16-bit | 1 = 32-bit
                    ; l: 0 = not 64-bit | 1 = long mode
                    ; avl: 0 = free for any code | 1 = reserved
                    ; limit 19:16 = 0xFFFF
    db 0x00         ; base 31:24 (0x00)     high  

GDT_DATA_KERNEL_SEGMENT:                                        ;data segment
    dw 0xFFFF       ; limit 15:0 (0xFFFF + 0xf below = 0xFFFF)
    dw 0x0000       ; base 31:16 (0x0000)   little
    db 0x00         ; base 23:16 (0x00)     medium
    db 0b10010010   ; access byte:
                    ; p: 0 = not present (fault) | 1=present
                    ; dpl: 00 = ring0 | 01 = ring1 | 10 = ring2 | 11 = ring3
                    ; s: 0 = system | 1 = code/data
                    ; type = 1010:
                    ;   code/data: 0 = data | 1 = code 
                    ;   conforming: 0 = non-conforming | 1 = conforming
                    ;   readable: 0 = execute-only | 1 = readable
                    ;   accessed: 0 = not accessed | 1 = accessed (auto-set by cpu)
    db 0b11001111   ; flags:
                    ; g: 0 = bytes | 1 = 4kb pages
                    ; d/b: 0 = 16-bit | 1 = 32-bit
                    ; l: 0 = not 64-bit | 1 = long mode
                    ; avl: 0 = free for any code | 1 = reserved
                    ; limit 19:16 = 0xFFFF
    db 0x00         ; base 31:24 (0x00)     high                

;in descriptor GDT limit have size 20 bits: 16 little bits in (dw 0xFFFF), 4 seniors bits of sixth byte (db 0b0100|1111|<-limit) | segment size = 0b1100|1111|->:0xFFFF
;                                                                                                                                                        FFFF
;in descriptor GDT base have size 32 bits: little 16 bits, middle 8 bits, seniors 8 bits
;
;
;| 63..56 | 55..52 | 51..48 | 47..40 | 39..32 | 31..24 | 23..16 | 15..8 | 7..0 |
;|--------|--------|--------|--------|--------|--------|--------|-------|------|
;| Base   | Flags  | Limit  | Access | Base   | Base   | Base   | Limit | Limit|
;| 31..24 | G,D,L, | 19..16 | P,DPL, | 23..16 | 15..8  | 7..0   | 15..8 | 7..0 |
;|        | AVL    |        | S,Type |        |        |        |       |      |
GDT_END:
GDT_DESCRIPTOR_REGISTER:
dw GDT_END - GDT - 1
dd GDT
CODE_SEGMENT equ GDT_CODE_KERNEL_SEGMENT - GDT                         ;descriptor selector     | 0x08
DATA_SEGMENT equ GDT_DATA_KERNEL_SEGMENT - GDT                         ;descriptor selector     | 0x10  

SECOND_PART_MESSAGE db "Second part of Boot loader is loaded into memory on address ", 0x00
PM_SUCCESS_MSG db "Protected mode was installed successful...", 0x00 
newstr db 0x0a, 0x0d, 0x00
SPACE db " ", 0x00
times (0x0600 - ($ - PART_BOOTLOADER_START)) - 512 db 0  ;size