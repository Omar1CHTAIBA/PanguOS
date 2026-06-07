#include "utils.h"
#include "globals.h"

int starts_with(const char* str, const char* prefix) {
    while (*prefix) {
        if (*str != *prefix) return 0;
        str++; prefix++;
    }
    return 1;
}

int strings_match(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] || s2[i]) {
        if (s1[i] != s2[i]) return 0;
        i++;
    }
    return 1;
}

void itoa_custom(int n, char* buf) {
    if (n == 0) { buf[0] = '0'; buf[1] = 0; return; }
    int i = 0, is_neg = 0;
    if (n < 0) { is_neg = 1; n = -n; }
    while (n > 0) { buf[i++] = (n % 10) + '0'; n /= 10; }
    if (is_neg) buf[i++] = '-';
    buf[i] = 0;
    for (int j = 0; j < i / 2; j++) {
        char t = buf[j]; buf[j] = buf[i - 1 - j]; buf[i - 1 - j] = t;
    }
}

void build_anim_string() {
    char* text = "Welcome To panguOS"; 
    int i = 0;
    while(text[i]) { anim_str[i] = text[i]; i++; }
    anim_str[i] = 0;
    anim_len = i;
}

int eval_factor(char** p) {
    while(**p == ' ') (*p)++;
    if (**p == '(') {
        (*p)++;
        int res = eval_expr(p);
        if (**p == ')') (*p)++;
        return res;
    }
    int res = 0;
    while (**p >= '0' && **p <= '9') {
        res = res * 10 + (**p - '0');
        (*p)++;
    }
    return res;
}

int eval_term(char** p) {
    int res = eval_factor(p);
    while (1) {
        while(**p == ' ') (*p)++;
        if (**p == '*') { (*p)++; res *= eval_factor(p); }
        else if (**p == '/') { (*p)++; int div = eval_factor(p); if(div) res /= div; }
        else break;
    }
    return res;
}

int eval_expr(char** p) {
    int res = eval_term(p);
    while (1) {
        while(**p == ' ') (*p)++;
        if (**p == '+') { (*p)++; res += eval_term(p); }
        else if (**p == '-') { (*p)++; res -= eval_term(p); }
        else break;
    }
    return res;
}