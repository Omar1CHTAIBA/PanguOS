#include "types.h"
#include "globals.h"
#include "graphics.h"
#include "interrupts.h"

void kernel_main() {
    init_idt();
    init_mouse();
    
    __asm__ volatile("sti");

    while(1) {
        if(needs_redraw) {
            if(state == 0) { 
                rect(0, 0, 320, 200, 0); 
                draw_icon(152, 40, user_icon); 
                
                draw_text(60, 86, "USER:", 7); 
                rect(110, 80, 140, 20, (input_mode == 0 ? 15 : 8)); 
                draw_text(115, 86, username, (input_mode == 0 ? 0 : 15));
                
                draw_text(60, 121, "PASS:", 7); 
                rect(110, 115, 140, 20, (input_mode == 1 ? 15 : 8));
                for(int i=0; i<plen; i++) draw_char(115+(i*8), 121, '*', (input_mode == 1 ? 0 : 15));
            
            } else if (state == 1) { 
                rect(0, 0, 320, 200, 0); 
                int start_x = 160 - ((anim_char_count * 8) / 2); 
                for(int i = 0; i < anim_char_count; i++) {
                    draw_char(start_x + (i * 8), 96, anim_str[i], 15); 
                }

            } else if (state == 2) { 
                rect(0, 0, 320, 200, 3); 
                
                draw_icon(24, 20, game_icon);
                draw_text(12, 40, "GAMES", 15); 
                
                draw_icon(24, 80, term_icon);
                draw_text(16, 100, "TERM", 15);

                draw_ui_window(&win1, "GAMES");
                draw_window_contents(&win1);

                draw_ui_window(&term_win, "TERMINAL");
                if(!term_win.is_closed && !term_win.is_minimized) {
                    rect(term_win.x + 4, term_win.y + 20, term_win.w - 8, term_win.h - 24, 0); 
                    
                    for(int i = 0; i < term_cursor_y; i++) {
                        draw_text(term_win.x + 6, term_win.y + 24 + (i*10), term_buf[i], 10);
                    }
                    
                    int py = term_win.y + 24 + (term_cursor_y * 10);
                    int px = term_win.x + 6;
                    draw_text(px, py, username, 10); 
                    
                    int un_len = 0; while(username[un_len]) un_len++;
                    px += un_len * 8;
                    
                    draw_text(px, py, "> ", 10); px += 16;
                    draw_text(px, py, term_cmd, 15);
                }

                rect(0, 185, 320, 15, 7); 
                for(int i = 0; i < 320; i++) pixel(i, 185, 15);
                
                draw_3d_button(2, 187, 50, 11, 7);  
                draw_text(5, 189, "PANGU", 0); 
                
                if(win1.is_minimized && !win1.is_closed) { 
                    draw_3d_button(60, 187, 50, 11, 7); 
                    draw_text(65, 189, "GAMES", 0); 
                }

                if(!start_menu.is_closed){
                    draw_ui_window(&start_menu, "PANGU");
                    draw_text(start_menu.x + 8, start_menu.y + 22, "GAMES", 0);
                    draw_text(start_menu.x + 8, start_menu.y + 42, "TERMINAL", 0);
                    draw_text(start_menu.x + 8, start_menu.y + 62, "RESTART", 0);
                    draw_text(start_menu.x + 8, start_menu.y + 82, "SHUTDOWN", 0);

                    rect(start_menu.x + 6, start_menu.y + 36, 78, 1, 8);
                    rect(start_menu.x + 6, start_menu.y + 56, 78, 1, 8); 
                    rect(start_menu.x + 6, start_menu.y + 76, 78, 1, 8); 
                }
            }
            
            if(state == 2) draw_char(mouse_x, mouse_y, '^', 0); 
            
            flip(); 
            needs_redraw = 0;
        }
        
        __asm__ volatile("hlt");
    }
}