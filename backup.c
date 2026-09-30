#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "eval.h"

bool contains_expr(char **expressions, int max_expr, char *expr, char *end, int *num_expr){
    bool equal; 
    
    if (!num_expr)
        return false;
    for (char **p = expressions; p < expressions + *num_expr; p++){
        for (char *q = expr, *r = *p; q <= end; q++, r++)
            if (*q != *r)
                break;
            else 
                if (q == end)
                    return true;
    }
    return false;
}
int add_expr(char **expressions, int max_expr, char *expr, char *end, int *num_expr){
    int len = end - expr; 
    char *new_expr = malloc(len);
    
    if (contains_expr(expressions, max_expr, expr, end, num_expr)){
        return 0;
    }
    
    if (*num_expr == max_expr){
        printf("Cannot add more than %d expressions.\n", max_expr);
        exit(EXIT_FAILURE);
    }
     
    //new_expr = malloc(len); 
    if (new_expr == NULL){
        printf("Insufficient memory.\n");
        free(new_expr);
        exit(EXIT_FAILURE);
    }
    
    for (char *p = expr, *q = new_expr; p <= end; p++, q++)
        *q = *p;
    
    expressions[(*num_expr)++] = new_expr;  
    
    return end - expr;
}

int store_expressions(char **expressions, int max_expr, char *expr, char *end, int *num_expr, int char_count){
    // Adds variables, NOT statements, and statements in parethensis.
    for (char *p = expr; *p != '\0'; p++){
        if (isalpha(*p)){
            char_count += add_expr(expressions, max_expr, p, p, num_expr); 
        } else if (*p == '~'){ 
            if (*(p + 1) == '('){ 
                char *end = find_right_para(p, p + strlen(expr)); 
                char_count += store_expressions(expressions, max_expr, p + 1, end, num_expr, char_count);
                if (!contains_expr(expressions, max_expr, p, end, num_expr)){
                    char_count += add_expr(expressions, max_expr, p, end, num_expr);
                }
                p = end - 1;
            } else if (isalpha(*(p + 1))){
                char_count += add_expr(expressions, max_expr, p, p + 1, num_expr);
                p += 1;
            } else {
                printf("Error: store_expressions tried to apply "
                "NOT to an incompatible character.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            if (*p == '('){
                char *end = find_right_para(p, p + strlen(expr)) - 1;
                add_expr(expressions, max_expr, p + 1, end, num_expr);
            }
        } 
        
    }
    printf("AND STATEMENTS\n");
    // Add AND statements
    for (char *prev, *p = expr; *p != '\0'; p++){  
        if (*p++ == '&'){ 
            if (*prev == ')'){
                prev = find_left_para(expr, prev);
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
            while (prev >= expr && !isalpha(*prev))
                prev--;
            
            if (prev <= expr && *(prev - 1) == '~')
                prev--;
            
          char_count += add_expr(expressions, max_expr, prev, p, num_expr);
          printf("Added at %s with end point %c\n", prev, *p);
        }
    }
    printf("OR STATEMENTS\n");
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
              char_count += add_expr(expressions, max_expr, prev, p, num_expr);
        }
    }
    return char_count;
}    
