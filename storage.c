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
    int len = end - expr + 1; 
    char *new_expr = malloc(len + 1);
    
    
    
    if (contains_expr(expressions, max_expr, expr, end, num_expr)){
        return 0;
    }
    
    if (*num_expr == max_expr){
        printf("Cannot add more than %d expressions.\n", max_expr);
        exit(EXIT_FAILURE);
    }
     
    if (new_expr == NULL){
        printf("Insufficient memory.\n");
        printf("Current expressions are:\n");
        
        for (int i = 0; i < *num_expr; i++)
            puts(expressions[i]);
        
        free(new_expr);
        exit(EXIT_FAILURE);
    }
    
    for (char *p = expr, *q = new_expr; p <= end; p++, q++)
        *q = *p;
    
    new_expr[len] = '\0';
    
    expressions[(*num_expr)++] = new_expr;  
    
    return end - expr;
}

char *get_next(char *expr, char *end){
    for (char *p = expr + 1; p <= end; p++)
        if (*p != ' ')
            return p;
    return end;
}
char *get_prev(char *start, char *cur){
    for (char *p = cur- 1; cur >= start; cur--)
        if (*p != ' ')
            return p;
    return start;
}

int store_expressions(char **expressions, int max_expr, char *expr, char *end, int *num_expr, int char_count){ 
    // Adds variables, NOT statements, and statements in parethensis.
    for (char *p = expr; p <= end && *p != '\0'; p++){ 
        if (isalpha(*p)){
            char_count += add_expr(expressions, max_expr, p, p, num_expr); 
        } else if (*p == '~'){ 
            if (*get_next(p, end) == '('){ 
                char *end = find_right_para(p, p + strlen(expr)); 
                char_count += store_expressions(expressions, max_expr, p + 1, end, num_expr, char_count);
            } else if (isalpha(*get_next(p, end))){
                char_count += add_expr(expressions, max_expr, p, get_next(p, end), num_expr);
            } else {
                printf("Error: store_expressions tried to apply "
                "NOT to incompatible character %c.\n", *p);
                exit(EXIT_FAILURE);
            }
        } else {
            if (*p == '('){
                char *para = find_right_para(p, end);
                add_expr(expressions, max_expr, p + 1, para - 1, num_expr);
            }
        } 
        
    }
    
    // Add AND statements
    for (char *prev, *cur, *p = expr; p <= end && *p != '\0'; p++){  
        if (*p == '&'){  
            // Find the start of the and expression
            for (cur = p, prev = p; cur >= expr && (*cur == ' ' || *cur == '&' || *cur == '(' || *cur == ')' || *cur == '~' || isalpha(*cur)); cur--){
                if (isalpha(*cur) || *cur == '(' || *cur == '~')
                    prev = cur;
            }
            
            // Find the end of the and expression
            for (char *q = p; q <= end && (*q == '&' || *q == ' ' || *q == '(' || *q == ')' || *q == '~' || isalpha(*q)); q++)
                if (*q != ' ')
                    p = q;
            
            if (*end == '~')
                p = get_next(p, end);
             
            // If the end entered a parentheses, go to the end
            if (p <= end && *get_prev(expr, p) == '('){
                while (p >= expr && *get_prev(expr, p) == '(')
                    p = get_prev(expr, p);
                p = find_right_para(p, end);
            } 
            char_count += add_expr(expressions, max_expr, prev, p, num_expr); 
        }
    }   
    // Add OR statements
    for (char *prev, *cur, *p = expr; p <= end && *p != '\0'; p++){  
        if (*p == '|'){  
            // Find the start of the and expression
            for (cur = p, prev = p; cur >= expr && (*cur == ' ' || *cur == '&' || 
                *cur == '(' || *cur == ')' || *cur == '~' || *cur == '|' ||
                isalpha(*cur)); cur--){
                if (isalpha(*cur) || *cur == '(' || *cur == '~')
                    prev = cur;
            }
            
            // Find the end of the and expression
            for (char *q = p; q <= end && (*q == '&' || *q == ' ' || *q == '(' || *q == ')' || 
                *q == '~' || *q == '|' || isalpha(*q)); q++)
                if (*q != ' ')
                    p = q;
            
            if (*end == '~')
                p = get_next(p, end);
            
            // If the end entered a parentheses, go to the end
            if (p <= expr && *get_prev(expr, p) == '('){
                while (p >= expr && *get_prev(expr, p) == '(')
                    p = get_prev(expr, p);
                p = find_right_para(p, end);
            }
            char_count += add_expr(expressions, max_expr, prev, p, num_expr); 
        }
    }
    return char_count;
}    
