#include "interrupts.h"
#include "globals.h"
#include "games.h"
#include "utils.h"
#include "io.h"

struct idt_entry {
    u16 base_lo;
    u16 sel;
    u8  always0;
    u8  flags;
    u16 base_hi;
} __attribute__((packed));

struct idt_ptr {
    u16 limit;
    u32 base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

void set_idt_gate(u8 num, u32 base, u16 sel, u8 flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel     = sel;
    idt[num].always0 = 0;
    idt[num].flags   = flags;
}

void init_idt() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (u32)&idt;
    for(int i = 0; i < 32; i++) set_idt_gate(i, (u32)exception_handler_asm, 0x08, 0x8E);
    for(int i = 32; i < 256; i++) set_idt_gate(i, (u32)spurious_handler_asm, 0x08, 0x8E);

    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28); 
    outb(0x21, 0x04); outb(0xA1, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    outb(0x21, 0xF8); outb(0xA1, 0xEF); 

    set_idt_gate(0x20, (u32)timer_handler_asm, 0x08, 0x8E);
    set_idt_gate(0x21, (u32)keyboard_handler_asm, 0x08, 0x8E);
    set_idt_gate(0x2C, (u32)mouse_handler_asm, 0x08, 0x8E);

    __asm__ volatile("lidt %0" : : "m"(idtp));
}

void mouse_wait(u8 t) { u32 i=1000; if(t==0) while(i--) if(inb(0x64)&1) return; else while(i--) if(!(inb(0x64)&2)) return; }
void mouse_write(u8 w) { mouse_wait(1); outb(0x64, 0xD4); mouse_wait(1); outb(0x60, w); }

void init_mouse() { 
    mouse_wait(1); 
    outb(0x64, 0xA8); 
    mouse_wait(1);
    outb(0x64, 0x20); 
    mouse_wait(0);
    u8 status = inb(0x60);
    status |= 2; 
    status &= ~0x20; 
    mouse_wait(1);
    outb(0x64, 0x60); 
    mouse_wait(1);
    outb(0x60, status);
    mouse_write(0xF6); 
    inb(0x60); 
    mouse_write(0xF4); 
    inb(0x60); 
}

void timer_handler_c() {
    outb(0x20, 0x20); 
    
    if(state == 1) {
        if(anim_phase == 0) { 
            anim_char_count++;
            if(anim_char_count >= anim_len) { anim_phase = 1; anim_tick = 0; }
        } else if (anim_phase == 1) { 
            anim_tick++;
            if(anim_tick > 25) { anim_phase = 2; } 
        } else if (anim_phase == 2) { 
            anim_char_count--;
            if(anim_char_count <= 0) { state = 2; }
        }
        needs_redraw = 1;
    }

    if (state == 2 && active_game == 1 && snake_state == 0) {
        snake_timer++;
        if (snake_timer > 5) {
            snake_timer = 0;
            for (int i = snake_len - 1; i > 0; i--) {
                snake_x[i] = snake_x[i - 1];
                snake_y[i] = snake_y[i - 1];
            }
            if (snake_dir == 0) snake_y[0]--;
            if (snake_dir == 1) snake_x[0]++;
            if (snake_dir == 2) snake_y[0]++;
            if (snake_dir == 3) snake_x[0]--;

            if (snake_x[0] < 1 || snake_x[0] > 37 || snake_y[0] < 1 || snake_y[0] > 13) snake_state = 1;
            
            for (int i = 1; i < snake_len; i++) {
                if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) snake_state = 1;
            }

            if (snake_x[0] == food_x && snake_y[0] == food_y) {
                if (snake_len < 99) snake_len++;
                food_x = (snake_x[0] + 7) % 35 + 2; 
                food_y = (snake_y[0] + 5) % 11 + 2;
            }
            needs_redraw = 1;
        }
    }
}

void keyboard_handler_c() {
    u8 sc = inb(0x60);
    outb(0x20, 0x20); 
    
    static int shift_pressed = 0;
    if (sc == 0x2A || sc == 0x36) { shift_pressed = 1; return; } 
    if (sc == 0xAA || sc == 0xB6) { shift_pressed = 0; return; } 
    
    if(sc & 0x80) return; 

    if (state == 0) {
        if (sc == 0x48) { input_mode = 0; needs_redraw = 1; return; }
        if (sc == 0x50) { input_mode = 1; needs_redraw = 1; return; }
    }

    static char m[60]       = {0,0,'1','2','3','4','5','6','7','8','9','0','-','=','\b',0,'Q','W','E','R','T','Y','U','I','O','P','[',']','\n',0,'A','S','D','F','G','H','J','K','L',';','\'','`',0,'\\','Z','X','C','V','B','N','M',',','.','/',0,'*',0,' '};
    static char shift_m[60] = {0,0,'!','@','#','$','%','^','&','*','(',')','_','+','\b',0,'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,'A','S','D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V','B','N','M','<','>','?',0,'*',0,' '};
    
    char c = (sc < 60) ? (shift_pressed ? shift_m[sc] : m[sc]) : 0;
    
    if (state == 2 && active_game == 3 && !term_win.is_minimized) {
        if (c == '\n') {
            term_execute();
        } else if (c == '\b') {
            if (term_cmd_len > 0) term_cmd[--term_cmd_len] = 0;
        } else if (c) {
            if (term_cmd_len < 30) {
                term_cmd[term_cmd_len++] = c;
                term_cmd[term_cmd_len] = 0;
            }
        }
        needs_redraw = 1;
        return; 
    }

    if(state == 0) { 
        if(c == '\n') { 
            if(input_mode == 0) {
                input_mode = 1; 
            } else if(input_mode == 1 && ulen > 0 && plen > 0) {
                if (!is_registered) {
                    for(int i = 0; i < 16; i++) {
                        saved_user[i] = username[i];
                        saved_pass[i] = password[i];
                    }
                    is_registered = 1;
                    build_anim_string();
                    state = 1; anim_char_count = 0; anim_phase = 0; anim_tick = 0;
                } else {
                    if (strings_match(username, saved_user) && strings_match(password, saved_pass)) {
                        build_anim_string();
                        state = 1; anim_char_count = 0; anim_phase = 0; anim_tick = 0;
                    } else {
                        ulen = 0; plen = 0;
                        username[0] = 0; password[0] = 0;
                        input_mode = 0;
                    }
                }
            }
        }
        else if(c == '\b') { if(input_mode==0 && ulen>0) username[--ulen]=0; else if(input_mode==1 && plen>0) password[--plen]=0; }
        else if(c) { if(input_mode==0 && ulen<14) username[ulen++]=c; else if(input_mode==1 && plen<14) password[plen++]=c; }
        needs_redraw = 1;
    }

    if (state == 2 && active_game == 1 && snake_state == 0) {
        if ((c == 'W' || c == 'w') && snake_dir != 2) snake_dir = 0;
        if ((c == 'A' || c == 'a') && snake_dir != 1) snake_dir = 3;
        if ((c == 'S' || c == 's') && snake_dir != 0) snake_dir = 2;
        if ((c == 'D' || c == 'd') && snake_dir != 3) snake_dir = 1;
    }
}

void mouse_handler_c() {
    u8 status = inb(0x64);
    if(status & 0x20) { 
        u8 data = inb(0x60);
        if (mouse_cycle == 0 && !(data & 0x08)) { outb(0xA0, 0x20); outb(0x20, 0x20); return; }

        mouse_packet[mouse_cycle++] = data;
        
        if(mouse_cycle == 3) {
            mouse_cycle = 0; 
            l_click = (mouse_packet[0] & 1);
            mouse_x += (signed char)mouse_packet[1]; 
            mouse_y -= (signed char)mouse_packet[2];
            if(mouse_x<0)mouse_x=0; if(mouse_x>312)mouse_x=312; 
            if(mouse_y<0)mouse_y=0; if(mouse_y>192)mouse_y=192;

            if(state == 0 && l_click) {
                if(mouse_x >= 110 && mouse_x <= 250) {
                    if(mouse_y >= 80 && mouse_y <= 100) input_mode = 0;
                    else if(mouse_y >= 115 && mouse_y <= 135) input_mode = 1;
                }
            }

            if(state == 2) { 
                if(l_click && mouse_x >= 24 && mouse_x <= 40 && mouse_y >= 20 && mouse_y <= 36) { 
                    win1.is_closed = 0; win1.is_minimized = 0; active_game = 0; 
                }
                
                if(l_click && mouse_x >= 24 && mouse_x <= 40 && mouse_y >= 80 && mouse_y <= 96) {
                    term_win.is_closed = 0; term_win.is_minimized = 0; active_game = 3;
                }
                
                if(l_click && !win1.is_closed && !win1.is_minimized) {
                    if(mouse_x > win1.x+win1.w-16 && mouse_x < win1.x+win1.w-4 && mouse_y > win1.y+5 && mouse_y < win1.y+15) {
                        win1.is_closed = 1; if(active_game == 1 || active_game == 2) active_game = 0; 
                    }
                    else if(mouse_x > win1.x+win1.w-29 && mouse_x < win1.x+win1.w-17 && mouse_y > win1.y+5 && mouse_y < win1.y+15) {
                        win1.is_minimized = 1;
                    }
                    else if(active_game == 0 && mouse_y < win1.y+18 && mouse_x > win1.x && mouse_x < win1.x+win1.w) {
                        win1.is_dragging = 1;
                    }
                    
                    if(!win1.is_dragging) {
                        if (active_game == 0) {
                            if(mouse_x >= win1.x+28 && mouse_x <= win1.x+44 && mouse_y >= win1.y+28 && mouse_y <= win1.y+44) {
                                active_game = 1; init_snake();
                            }
                            if(mouse_x >= win1.x+98 && mouse_x <= win1.x+114 && mouse_y >= win1.y+28 && mouse_y <= win1.y+44) {
                                active_game = 2; init_ttt();
                            }
                        } else if (active_game == 2 && ttt_winner == 0) {
                            int cx = (mouse_x - win1.x - 53) / 18;
                            int cy = (mouse_y - win1.y - 24) / 18;
                            if (cx >= 0 && cx < 3 && cy >= 0 && cy < 3) {
                                int idx = cy * 3 + cx;
                                if (ttt_board[idx] == 0) {
                                    ttt_board[idx] = ttt_player;
                                    ttt_player = (ttt_player == 1) ? 2 : 1;
                                    int wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
                                    for(int w=0; w<8; w++) {
                                        if(ttt_board[wins[w][0]] != 0 && ttt_board[wins[w][0]] == ttt_board[wins[w][1]] && ttt_board[wins[w][1]] == ttt_board[wins[w][2]]) {
                                            ttt_winner = ttt_board[wins[w][0]];
                                        }
                                    }
                                }
                            }
                        } else if (active_game != 0 && (ttt_winner != 0 || snake_state != 0)) {
                            active_game = 0; 
                        }
                    }
                }
                
                if(l_click && !term_win.is_closed && !term_win.is_minimized) {
                    if(mouse_x > term_win.x+term_win.w-16 && mouse_x < term_win.x+term_win.w-4 && mouse_y > term_win.y+5 && mouse_y < term_win.y+15) {
                        term_win.is_closed = 1; if(active_game == 3) active_game = 0; 
                    }
                    else if(mouse_y < term_win.y+18 && mouse_x > term_win.x && mouse_x < term_win.x+term_win.w) {
                        term_win.is_dragging = 1;
                    }
                }
                
                if(!l_click) { win1.is_dragging = 0; term_win.is_dragging = 0; }
                if(win1.is_dragging) { win1.x = mouse_x - (win1.w/2); win1.y = mouse_y - 6; }
                if(term_win.is_dragging) { term_win.x = mouse_x - (term_win.w/2); term_win.y = mouse_y - 6; }
                
                if(l_click && mouse_y >= 187 && mouse_x >= 60 && mouse_x <= 110) win1.is_minimized = 0;
                
                if(l_click && mouse_x >= 2 && mouse_x <= 52 && mouse_y >= 187 && mouse_y <= 198) {start_menu.is_closed = !start_menu.is_closed;}
                
                if(!start_menu.is_closed && l_click){
                    int rx = mouse_x - start_menu.x;
                    int ry = mouse_y - start_menu.y;
                    if(rx >= 6 && rx <= 80 && ry >= 20 && ry <= 32){ 
                        win1.is_closed = 0; win1.is_minimized = 0; start_menu.is_closed = 1; 
                    }
                    else if(rx >= 6 && rx <= 80 && ry >= 40 && ry <= 52){ 
                        term_win.is_closed = 0; term_win.is_minimized = 0; active_game = 3; start_menu.is_closed = 1; 
                    }
                    else if(rx >= 6 && rx <= 80 && ry >= 60 && ry <= 72){
                        start_menu.is_closed = 1; input_mode = 0; ulen = 0; plen = 0;
                        username[0] = 0; password[0] = 0; state = 0;              
                    }
                    else if(rx >= 6 && rx <= 80 && ry >= 80 && ry <= 92){ outw(0x604, 0x2000); }
                }
                if(l_click && !start_menu.is_closed){
                    if(mouse_x < start_menu.x || mouse_x > start_menu.x + start_menu.w || mouse_y < start_menu.y || mouse_y > start_menu.y + start_menu.h){
                        if(!(mouse_y >= 187 && mouse_x >= 2 && mouse_x <= 52)) start_menu.is_closed = 1;
                    }
                }
            }
            needs_redraw = 1;
        }
    }
    outb(0xA0, 0x20); 
    outb(0x20, 0x20); 
}