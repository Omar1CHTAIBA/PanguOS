#ifndef IO_H
#define IO_H

#include "types.h"

static inline void outw(u16 p, u16 v) { 
    __asm__ volatile("outw %0, %1" : : "a"(v), "Nd"(p)); 
}
static inline u8 inb(u16 p) { 
    u8 r; 
    __asm__ volatile("inb %1, %0" : "=a"(r) : "Nd"(p)); 
    return r; 
}
static inline void outb(u16 p, u8 v) { 
    __asm__ volatile("outb %0, %1" : : "a"(v), "Nd"(p)); 
}

#endif