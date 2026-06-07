#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

void set_idt_gate(u8 num, u32 base, u16 sel, u8 flags);
void init_idt();

void mouse_wait(u8 t);
void mouse_write(u8 w);
void init_mouse();

void timer_handler_c();
void keyboard_handler_c();
void mouse_handler_c();

extern void timer_handler_asm();
extern void keyboard_handler_asm();
extern void mouse_handler_asm();
extern void exception_handler_asm(); 
extern void spurious_handler_asm();

#endif