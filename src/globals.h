#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"

extern u8* VGA;
extern u8* back_buffer;

extern struct Window win1;
extern struct Window start_menu;
extern struct Window term_win;

extern volatile int state;         
extern volatile int input_mode;    
extern volatile int needs_redraw;

extern char username[16]; 
extern char password[16];
extern char saved_user[16]; 
extern char saved_pass[16];
extern int is_registered;
extern int ulen, plen;

extern char anim_str[64];
extern int anim_len;
extern volatile int anim_char_count;
extern volatile int anim_phase; 
extern volatile int anim_tick;

extern int mouse_x, mouse_y;
extern u8 mouse_cycle, mouse_packet[3];
extern int l_click;

extern int active_game; 

extern int snake_x[100], snake_y[100];
extern int snake_len, snake_dir; 
extern int food_x, food_y;
extern int snake_timer, snake_state; 

extern int ttt_board[9]; 
extern int ttt_player;
extern int ttt_winner; 

extern char term_buf[10][40]; 
extern char term_cmd[32];
extern int term_cmd_len;
extern int term_cursor_y;

#endif