#ifndef TYPES_H
#define TYPES_H

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

struct Window {
    int x, y, w, h, is_minimized, is_closed, is_dragging, is_not_static, has_minimize;
}; 

#endif