#ifndef UTILS_H
#define UTILS_H

int starts_with(const char* str, const char* prefix);
int strings_match(const char* s1, const char* s2);
void itoa_custom(int n, char* buf);
void build_anim_string();

int eval_expr(char** p);
int eval_term(char** p);
int eval_factor(char** p);

#endif