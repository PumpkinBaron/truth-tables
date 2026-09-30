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
bool run_expr(char *expr, bool *char_vals, int len, int num_vars);
bool eval_expr(char *expr, char *end);

char *find_right_para(char *cur, char *end){ 
    int left_para = 0;
    
    if (*cur == '~')
        cur++;
    
    if (*cur == '(')
        cur++;
    
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
    for (char *p = cur; p >= start; p--){
        if (*p == ')')
            right_para++;
        if (*p == '(')
            right_para--;
        if (right_para < 0)
            return p;
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
    return isalpha(c) || c == '0' || c == '1' || c == '&' || c == '(' || c == ')' || c == '~' || c == ' ';
 }

bool run_expr(char *expr, bool *char_vals, int len, int num_vars){
    char dup_expr[len + 1];
    strcpy(dup_expr, expr);
    
    // If the whole expression is in parenthesis, remove them.
    if (*(expr + len - 1) == ')' && *expr == ')'){
        dup_expr[len] = '\0';
        dup_expr[0] = ' ';
        len--;
        expr++;
    }
    
    // Replace variables with 1 or 0. 
    for (int i = 0; i < len; i++){
        if (isalpha(dup_expr[i])){
            if (char_vals[toupper(dup_expr[i]) - 65])
                dup_expr[i] = '1';
            else
                dup_expr[i] = '0';
        }
    }
    
    return eval_expr(dup_expr, dup_expr + len);
}


void eval_paren(char *expr, char *end, char *next_val){
    char *right_para; 
    for (char *p = expr; p <= end; p++){
        if (*p == '('){
            right_para = find_right_para(p, end);
            *p = ' ';
            *right_para = ' ';
            eval_expr(++p, --right_para);
        }
    } 
}


void eval_not(char *expr, char *end, char *next_val){ 
    for (char *p = expr; p <= end && *p != '\0'; p++){ 
        
        if (*p == '~') {
            next_val = find_next_var(p, end);
            *p = ' ';
            if (*next_val == '1')
                *next_val = '0';
            else
                *next_val = '1';
        }
    } 
}


void eval_and(char *expr, char *end, char *next_val){
    bool prev_bool, found_op = false;
    char *prev, *op; 
    for (char *p = expr; p <= end; p++){ 
        if (found_op && (*p == '1' || *p == '0')){
            found_op = false;
            *op = ' ';
            if (*p == '0' || *prev == '0'){
                *p = ' ';
                *prev = '0';
            } else {
                *p = ' ';
                *prev = '1';
            }
        }
        if (*p == '&'){
            found_op = true;
            op = p;
        } 
        if (*p == '1' || *p == '0')
            prev = p; 
    }
}


void eval_or(char *expr, char *end, char *next_val){
    bool prev_bool, found_op = false;
    char *prev, *op; 
    for (char *p = expr; p <= end; p++){ 
        if (found_op && (*p == '1' || *p == '0')){
            found_op = false;
            *op = ' ';
            if (*p == '1' || *prev == '1'){
                *p = ' ';
                *prev = '1';
            } else {
                *p = ' ';
                *prev = '0';
            }
        }
        if (*p == '|'){
            found_op = true;
            op = p;
        } 
        if (*p == '1' || *p == '0')
            prev = p; 
    }
}

 
bool eval_expr(char *expr, char *end){
    char *next_val; 
    if (*expr == '('){
        *expr = ' ';
        *end = ' ';
    }
    
    eval_paren(expr, end, next_val);
    eval_not(expr, end, next_val);
    eval_and(expr, end, next_val); 
    eval_or(expr, end, next_val);  
      
    
    if (*find_next_var(expr, end) == '1')
        return true;
    return false;
}

