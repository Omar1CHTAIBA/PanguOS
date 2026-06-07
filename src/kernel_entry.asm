[BITS 32]

global _start
extern kernel_main

; External C Handlers
extern timer_handler_c
extern keyboard_handler_c
extern mouse_handler_c

; Global Assembly Wrappers
global timer_handler_asm
global keyboard_handler_asm
global mouse_handler_asm
global exception_handler_asm
global spurious_handler_asm

_start:
    call kernel_main

.loop:
    hlt
    jmp .loop

; =========================================
; SAFE INTERRUPT SERVICE ROUTINES
; =========================================

; Catch-all for CPU exceptions (prevents triple fault reboots)
exception_handler_asm:
    cli
    hlt
    jmp exception_handler_asm

; Catch-all for ghost/spurious hardware interrupts
spurious_handler_asm:
    pushad
    mov al, 0x20
    out 0x20, al    ; Send EOI to master just in case
    popad
    iretd

; Hardware Interrupts with strict C ABI stack alignment
timer_handler_asm:
    pushad
    cld                  ; Clear direction flag for string operations
    mov eax, esp         ; Save original stack pointer
    and esp, 0xFFFFFFF0  ; Force 16-byte alignment
    sub esp, 12          ; Pad stack to account for the next push
    push eax             ; Push original ESP (Stack is now perfectly 16-byte aligned)
    call timer_handler_c
    pop esp              ; Restore exact ESP from before alignment
    popad
    iretd

keyboard_handler_asm:
    pushad
    cld
    mov eax, esp
    and esp, 0xFFFFFFF0
    sub esp, 12
    push eax
    call keyboard_handler_c
    pop esp
    popad
    iretd

mouse_handler_asm:
    pushad
    cld
    mov eax, esp
    and esp, 0xFFFFFFF0
    sub esp, 12
    push eax
    call mouse_handler_c
    pop esp
    popad
    iretd
