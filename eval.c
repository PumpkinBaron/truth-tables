#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <ctype.h>
#include <string.h>
#include "eval.h"
#include "storage.h"
// Evalautes a string containing a logic expression. It
// works by evaluating in the order of ()'s, ~ (NOT), & (AND),
// | (OR) and replaces evaluated sub-expressions with a space. 

char *find_left_para(char *start, char *cur);
char *find_left_para(char *start, char *cur);
char *find_next_var(char *start, char *end);
bool within_and(char c);
char *find_and_start(char *expr, char *cur);
bool run_expr(char *expr, bool *char_vals, int len, int num_vars);
bool eval_expr(char *expr, char *end);

char *find_right_para(char *cur, char *end){
    int left_para = 0;
    for (char *p = cur; p <= end; p++){
        if (*p == '(')
            left_para++;
        if (*p == ')')
            left_para--;
        if (left_para < 0)
            return p;
    }
    printf("ERROR: No right parenthesis found.\n");
    exit(EXIT_FAILURE);
}

char *find_left_para(char *start, char *cur){
    int right_para = 0;
    while (cur-- >= start){
        if (*cur == ')')
            right_para++;
        if (*cur == '(')
            right_para--;
        if (right_para < 0)
            return cur;
    }
    printf("Failed to find left parenthesis.\n");
    exit(EXIT_FAILURE);
}

char *find_next_var(char *start, char *end){
    for (char *p = start; p <= end; p++){
        if (*p == '0' || *p == '1')
            return p;
    }
    printf("ERROR: Couldn't find next value.\n");    
    exit(EXIT_FAILURE);
}

bool within_and(char c){
    return isalpha(c) | (c == '&') | (c == '(') | (c == ')') | (c == '~');
 }

char *find_and_start(char *expr, char *cur){
    char *start;
    int right_paras = 0;
    while (cur-- != expr && within_and(*(cur -1))){
        if (*cur == ')')
            right_paras++;
        if (*cur == '(')
            right_paras--;
        if (right_paras < 0)
            break;
        if (*cur == '&')
            start = cur - 1;
    }
    return start;
}

bool run_expr(char *expr, bool *char_vals, int len, int num_vars){
    char dup_expr[len + 1];
    strcpy(dup_expr, expr);
    
    // Replace variables with 1 or 0. 
    for (int i = 0; i < len; i++){
        if (isalpha(dup_expr[i])){
            if (char_vals[toupper(dup_expr[i]) - 65])
                dup_expr[i] = '1';
            else
                dup_expr[i] = '0';
        }
    }
    return eval_expr(dup_expr, dup_expr + strlen(dup_expr));
}


bool eval_expr(char *expr, char *end){
    bool prev_bool, found_op;
    int prev_ind, op_ind;
    char *next_val, *prev, *op; 
    if (*expr == '('){
        *expr = ' ';
        *end = ' ';
    }
    // Evaluates expressions inside parenthesis
    for (char *p = expr; p <= end; p++){
        if (*p == '('){
            char *right_para = find_right_para(p, end);
            eval_expr(expr, right_para);
        }
    }
    // Applies NOT
    for (char *p = expr; p <= end; p++){
        if (*p == '~') {
            next_val = find_next_var(p, end);
            if (*next_val == '0')
                *next_val = '1';
            else if (*next_val == '1')
                *next_val = '1';
        }
    }
    
    // Evaluates & and |
    for (char *p = expr; p <= end; p++){
        if (*p == '0'){
            if (found_op){
                found_op = false;
                *p = ' ';
                if (*op == '&'){
                    expr[prev_ind] = '0';
                    prev_bool = false;
                }
                else if (*op == '|'){
                    if (prev_bool)
                        expr[prev_ind] = '1';
                    else {
                        expr[prev_ind] = '0';
                        prev_bool = false;
                    }
                }
                *op = ' ';
            } else {
                prev = p;
                prev_bool = false;
            }
        }
        if (*p == '1'){
            if (found_op){
                found_op = false;
                *p = ' ';
                if (*op == '&')
                    if (prev_bool)
                        *p = '1';
                    else {
                        *p = '0';
                        prev_bool = false;
                    }
                else if (*op == '|'){
                    *p = '1';
                    prev_bool = true;
                }
                *op = ' ';
            } else {
                prev = p;
                prev_bool = true;
            }
        }
        if (*p == '&' | *p == '|'){
            found_op = true;
            op = p;
        }
    }
    if (*find_next_var(expr, end) == '1')
        return true;
    return false;
}

