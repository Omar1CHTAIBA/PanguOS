#include "games.h"
#include "globals.h"
#include "utils.h"

void term_scroll() {
    for(int i = 0; i < 9; i++) {
        int j = 0;
        while(term_buf[i+1][j]) { term_buf[i][j] = term_buf[i+1][j]; j++; }
        term_buf[i][j] = 0;
    }
    term_buf[9][0] = 0;
}

void term_print(const char* s) {
    if (term_cursor_y < 10) {
        int i = 0;
        while(s[i] && i < 39) { term_buf[term_cursor_y][i] = s[i]; i++; }
        term_buf[term_cursor_y][i] = 0;
        term_cursor_y++;
    } else {
        term_scroll();
        int i = 0;
        while(s[i] && i < 39) { term_buf[9][i] = s[i]; i++; }
        term_buf[9][i] = 0;
    }
}

void term_execute() {
    char history_line[64] = {0};
    int idx = 0;
    int u = 0;
    while(username[u] && idx < 15) history_line[idx++] = username[u++];
    history_line[idx++] = '>';
    history_line[idx++] = ' ';
    int c_idx = 0;
    while(term_cmd[c_idx] && idx < 58) history_line[idx++] = term_cmd[c_idx++];
    history_line[idx] = 0;
    
    term_print(history_line);
    
    if (strings_match(term_cmd, "CLS")) {
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 40; j++) {
                term_buf[i][j] = 0;
            }
        }
        term_cursor_y = 0;
    }
    
    else if (starts_with(term_cmd, "CHANGE --P TO ")) {
        int i = 0;
        while(term_cmd[14+i] && i < 14) { 
            password[i] = term_cmd[14+i]; 
            saved_pass[i] = term_cmd[14+i];
            i++; 
        }
        password[i] = 0; saved_pass[i] = 0;
        plen = i;
        term_print("Password updated successfully.");
    } 
    else if (starts_with(term_cmd, "CALCULATE ")) {
        char* ptr = term_cmd + 10;
        int res = eval_expr(&ptr);
        
        char rstr[16];
        itoa_custom(res, rstr);
        
        char out[40] = ">>> ";
        int j = 4, k = 0;
        while(rstr[k] && j < 38) out[j++] = rstr[k++];
        out[j] = 0;
        
        term_print(out);
    } 
    else if (strings_match(term_cmd, "PANGU CREATOR")) {
        term_print("Chtaiba Omar");
    } 
    else if (term_cmd_len > 0) {
        term_print("Unknown command.");
    }
    
    term_cmd[0] = 0; 
    term_cmd_len = 0;
}

void init_snake() {
    snake_len = 3; snake_dir = 1; snake_state = 0;
    snake_x[0] = 10; snake_y[0] = 5;
    snake_x[1] = 9;  snake_y[1] = 5;
    snake_x[2] = 8;  snake_y[2] = 5;
    food_x = 20; food_y = 10;
}

void init_ttt() {
    for(int i=0; i<9; i++) ttt_board[i] = 0;
    ttt_player = 1; ttt_winner = 0;
}