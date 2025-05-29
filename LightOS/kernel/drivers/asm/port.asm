global in8, out8, in16, out16, in32, out32

[bits 32]

;================================================================================================================
; 8-bit input
in8:               ;uint8 in8(uint8 port)
push ebp
mov ebp, esp

mov edx, [ebp+8]
in eax, dx

mov esp, ebp
pop ebp
ret

; 8-bit output
out8:                ;void out8(uint8 port, uint8 value)      
push ebp             
mov ebp, esp

mov edx, [ebp+8]
mov eax, [ebp+12]
out dx, al

mov esp, ebp
pop ebp
ret
;================================================================================================================

;================================================================================================================
;32 - bit input
in32:                                ;uint32 in32(uint32 port)         
push ebp
mov ebp, esp

mov edx, [ebp+8]
in eax, dx

mov esp, ebp
pop ebp
ret

; 32-bit output
out32:                               ;void out32(uint32 port, uint32 value)
push ebp
mov ebp, esp

mov edx, [ebp+8]                    ;16 bit address bus
mov eax, [ebp+12]                  ; 
out dx, al

mov esp, ebp
pop ebp
ret
;================================================================================================================

;================================================================================================================
; 16-bit input                                                                                                                      ;BAGS
in16:               ;uint16 in16(uint16 port)
push ebp
mov ebp, esp

mov edx, [ebp+8]
in eax, dx

mov esp, ebp
pop ebp
ret

; 16-bit output
out16:                ;void out16(uint16 port, uint16 value)      
push ebp             
mov ebp, esp

mov edx, [ebp+8]
mov eax, [ebp+12]
out dx, ax

mov esp, ebp
pop ebp
ret
;================================================================================================================