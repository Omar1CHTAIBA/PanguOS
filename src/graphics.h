#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"

extern unsigned char font[46][8];
extern u8 game_icon[256];
extern u8 user_icon[256];
extern u8 term_icon[256];
extern u8 snake_icon[256];
extern u8 ttt_icon[256];

void pixel(int x, int y, u8 c);
void rect(int x, int y, int w, int h, u8 c);
void draw_char(int x, int y, char c, u8 color);
void draw_text(int x, int y, const char* s, u8 c);
void draw_icon(int x, int y, u8* data);
void flip();
void draw_3d_button(int x, int y, int w, int h, u8 bg_color);
void draw_ui_window(struct Window* w, const char* title);
void draw_window_contents(struct Window* w);

#endif