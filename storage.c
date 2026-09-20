#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "eval.h"

bool contains_expr(char **expressions, int max_expr, char *expr, char *end, int *num_expr){ 
    if (!num_expr)
        return false;
    for (char **p = expressions; p < expressions + *num_expr; p++){
        printf("Comparing %s and %s\n", *p, expr);
        if (!strcmp(*p, expr))
            return true;
    }
    return false;
}
void add_expr(char **expressions, int max_expr, char *expr, char *end, int *num_expr){
    int len = end - expr; 
    
    if (contains_expr(expressions, max_expr, expr, end, num_expr)){
     printf("Already contained.\n");
        return;
    }
    
    if (*num_expr == max_expr){
        printf("Cannot add more than %d expressions.\n", max_expr);
        exit(EXIT_FAILURE);
    }
    char *new_expr = malloc(len);
    if (new_expr == NULL){
        printf("Insufficient memory.\n");
        free(new_expr);
        exit(EXIT_FAILURE);
    }
    for (char *p = new_expr, *q = expr; p <= new_expr + len; p++, q++)
        *p = *q;
    expressions[(*num_expr)++] = new_expr;
}

int store_expressions(char **expressions, int max_expr, char *expr, char *end, int *num_expr){
    // Adds variables, NOT statements, and statements in parethensis.
    for (char *p = expr; *p != '\0'; p++){
        if (isalpha(*p)){
            char var[2] = {*p, '\0'};
            add_expr(expressions, max_expr, var, var + 1, num_expr);
        } else if (*p == '~'){
            if (*(p + 1) == '('){
                char *end = find_right_para(p, p + strlen(expr));
                store_expressions(expressions, max_expr, p + 1, end, num_expr);
                if (!contains_expr(expressions, max_expr, p, end, num_expr)){
                    add_expr(expressions, max_expr, p, end, num_expr);
                }
                p = end - 1;
            } else if (isalpha(*(p + 1) && !contains_expr(expressions, max_expr, p, p + 1, num_expr))){
                add_expr(expressions, max_expr, p, p + 1, num_expr);
                p += 1;
            } else {
                printf("Error: store_expressions tried to apply "
                "NOT to an incompatible character.\n");
                exit(EXIT_FAILURE);
            }
        } 
        
    }
    // Add AND statements
    for (char *p = expr; *p != '\0'; p++){
        char *prev = p - 1;
        if (*p++ == '&'){ 
            if (*prev == ')'){
                prev = find_left_para(expr, p);
                if (prev != expr && *(prev - 1) == '~')
                    // Include the NOT before the '('
                    prev--;
            }
            if (*p == '('){
                // Include the expression inside the parethensis
                p = find_right_para(p + 1, end);
            }
            if (prev != expr){
                prev = find_and_start(expr, p);
            }
            if (!contains_expr(expressions, max_expr, prev, p, num_expr))
                add_expr(expressions, max_expr, prev, p, num_expr);
        }                
    }
    // Add OR statements
    for (char *p = expr; *p != '\0'; p++){
        char *prev = p - 1;
        if (*p++ == '|'){ 
            if (*prev == ')'){
                prev = find_left_para(expr, p);
                if (prev != expr && *(prev - 1) == '~')
                    // Include the NOT before the '('
                    prev--;
            }
            if (*p == '('){
                // Include the expression inside the parethensis
                p = find_right_para(p + 1, end);
            }
            if (prev != expr){
                prev = find_and_start(expr, p);
            }
            if (!contains_expr(expressions, max_expr, prev, p, num_expr))
                add_expr(expressions, max_expr, prev, p, num_expr);
        }
    }
}    
