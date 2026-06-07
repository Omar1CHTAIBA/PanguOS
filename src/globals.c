#include "globals.h"

u8* VGA = (u8*)0xA0000;
u8* back_buffer = (u8*)0x50000;

struct Window win1 = {70, 50, 160, 80, 0, 1, 0, 1, 1};
struct Window start_menu = {2, 75, 90, 110, 0, 1, 0, 0, 0};
struct Window term_win = {10, 20, 300, 140, 0, 1, 0, 1, 0}; 

volatile int state = 0;         
volatile int input_mode = 0;    
volatile int needs_redraw = 1;

char username[16] = {0}; 
char password[16] = {0};
char saved_user[16] = {0}; 
char saved_pass[16] = {0};
int is_registered = 0;
int ulen = 0, plen = 0;

char anim_str[64] = {0};
int anim_len = 0;
volatile int anim_char_count = 0;
volatile int anim_phase = 0; 
volatile int anim_tick = 0;

int mouse_x = 160, mouse_y = 100;
u8 mouse_cycle = 0, mouse_packet[3];
int l_click = 0;

int active_game = 0; 

int snake_x[100], snake_y[100];
int snake_len = 0, snake_dir = 1; 
int food_x = 0, food_y = 0;
int snake_timer = 0, snake_state = 0; 

int ttt_board[9] = {0}; 
int ttt_player = 1;
int ttt_winner = 0; 

char term_buf[10][40] = {{0}}; 
char term_cmd[32] = {0};
int term_cmd_len = 0;
int term_cursor_y = 0;