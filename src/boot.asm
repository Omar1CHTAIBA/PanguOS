[BITS 16]
[ORG 0x7C00]

KERNEL_OFFSET equ 0x1000

start:
    cli

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [BOOT_DRIVE], dl

; =========================================
; VGA MODE
; =========================================

    mov ax, 0x0013
    int 0x10

; =========================================
; LOAD KERNEL 
; (16 sectors safely fits in Track 0 without crossing boundaries)
; =========================================

    mov bx, KERNEL_OFFSET
    mov dh, 36 

    call disk_load

; =========================================
; GDT
; =========================================

    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEG:init_pm

; =========================================
; DISK LOAD
; =========================================

disk_load:

    pusha

    mov ah, 0x02
    mov al, dh
    mov ch, 0
    mov dh, 0
    mov cl, 2

    int 0x13

    jc disk_error

    popa
    ret

disk_error:
    jmp $

; =========================================
; GDT
; =========================================

gdt_start:

gdt_null:
    dq 0

gdt_code:
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0

gdt_data:
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; =========================================
; PMODE
; =========================================

[BITS 32]

init_pm:

    mov ax, DATA_SEG

    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call KERNEL_OFFSET

hang:
    hlt
    jmp hang

BOOT_DRIVE db 0

times 510-($-$$) db 0
dw 0xAA55
