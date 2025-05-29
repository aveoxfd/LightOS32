[org 0x7c00]                   ;our segment
SECOND_PART_LOADER equ 0x7e06

main:
xor ax, ax
mov ss, ax
mov sp, 0xFFFF                 ;stack 65KB
mov bp, sp

mov [BOOT_DRIVE], dl           ;BIOS (type of device booting)->dl

push HELLO_MSG
call print_string

push newstr                    
call print_string

push CODE_SEGMENT_TEXT
call print_string
push cs
call print_hex_word
push newstr
call print_string

push DATA_SEGMENT_TEXT
call print_string
push ds
call print_hex_word
push newstr
call print_string

push STACK_SEGMENT_TEXT
call print_string
push ss
call print_hex_word
push newstr
call print_string

push ax
call print_hex_word
push newstr
call print_string

push es
call print_hex_word
push newstr
call print_string

push fs
call print_hex_word
push newstr
call print_string

push gs
call print_hex_word
push newstr
call print_string

push 0x0000                     ;segment
push 0x0003                     ;count sectors
push 0x7e00                     ;address in RAM
push 0x0000                     ;cylinder
push 0x0000                     ;head
push 0x0002                     ;start from sector
mov ax, [BOOT_DRIVE]            
push ax                         ;type of device
call disk_rw

cmp eax, 0
je .succes
jmp .fail

.succes:
jmp .exit

.fail:
push SECTOR_LOAD_ERROR
call print_string
push newstr
call print_string
jmp $
.exit:

mov si, 0x7e00
mov word [si], disk_rw
mov si, 0x7e02
mov word [si], print_string
mov si, 0x7e04
mov word [si], print_hex_word

jmp SECOND_PART_LOADER          ;auto cs:offset

jmp $

;===========================================
print_hex_word:                ;print_hex_word(hex)
push bp
mov bp, sp

sub sp, 7
mov si, sp ; *buffer
mov byte [si+6], 0
mov byte [si], '0'
mov byte [si+1], 'x'
add si, 2

mov ax, [bp+4] ;FFFF 
mov bx, 0xF000 ;mask
mov cl, 0x0C ;->12
.loop:

cmp bx, 0
je .exit

mov ax, [bp+4]
and ax, bx ;F000
shr ax, cl
cmp al, 9
jle .digit
add al, 7

.digit:
add al, '0'
mov [si], al
inc si
sub cl, 4
shr bx, 4
jmp .loop

.exit:

push sp
call print_string
add sp, 7
mov sp, bp
pop bp
ret
;===========================================

;===========================================
print_string:                 ;print_string(*string)
push bp
mov bp, sp

mov si, [bp+4]
mov ah, 0x0e                  ;BIOS teletype

.loop:
mov al, [si]

cmp al, 0
je .exit

int 0x10
inc si

jmp .loop

.exit:
mov sp, bp
pop bp
ret
;============================================

;============================================
disk_rw:                  ;eax disk_rw(segment, count_sector, address_in_RAM, cylinder, header, start_sector, type_of_device)
push bp
mov bp, sp

mov ax, [bp+16]
mov es, ax                  ;es:offset(bx)

mov ah, 0x02                ;BIOS disk read vector interrupt
mov al, [bp+14]             ;count of sectors
mov bx, [bp+12]             ;address of destination in RAM es:bx  0x0000:0x7e00
mov ch, [bp+10]             ;cylinder
mov dh, [bp+8]              ;head
mov cl, [bp+6]              ;start reading from <?> sector
mov dl, [bp+4]              ;type of device (hdd, ssd, diskete, etc) 

int 0x13                    ;BIOS load from device

jc .disk_error              ;carry flag = error
jmp .done

.disk_error:                ;ah - code error, al - number of sectors read
mov ax, 0x0001
push SECTOR_LOAD_ERROR
call print_string
push newstr
call print_string
jmp $
jmp .extra_exit

.done:
push SECTOR_LOAD_SUCCESS
call print_string
mov ax, [bp+6]
push ax
call print_hex_word
push newstr
call print_string
xor ax, ax
.extra_exit:
mov sp, bp
pop bp
ret
;============================================

BOOT_DRIVE db 0
newstr db 0x0a, 0x0d, 0x00
HELLO_MSG db "Hello", 0x0a, 0x0d, "Starting boot...", 0x00
SECTOR_LOAD_ERROR db "Sector read error", 0x00
SECTOR_LOAD_SUCCESS db "Sector was read successful ", 0x00
STACK_SEGMENT_TEXT db "Stack segment register:", 0
CODE_SEGMENT_TEXT db "Code segment register: ", 0
DATA_SEGMENT_TEXT db "Data segment register: ", 0
times 510-($-$$) db 0
dw 0xaa55