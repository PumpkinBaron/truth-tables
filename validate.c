#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 1026

bool nested_correctly = true;
int parentheses = 0;

void paren_check(char c);
bool is_symbol(char c);
bool is_valid(char *expr);

void paren_check(char c){
    switch(c){
        case '(':
            parentheses++;
            break;
        case ')':
            if (parentheses == 0)
                nested_correctly = false;
            else 
                parentheses--;
            break;
    } 
}

bool is_symbol(char c){
    bool is_symb = false;
    switch(c){
        case '&':
            is_symb = true;
            break;
        case '|':
            is_symb = true;
            break;
    }
    return is_symb;
}

bool is_valid(char *expr){
    parentheses = 0;
    for (char *p = expr; p <= expr + strlen(expr) - 1; p++){
        while (*p == '(' || *p == ')'){
            paren_check(*p);
            p++;
        }
        char *prev = p - 1, *next = p + 1;
        
        while (p != expr && *prev == '(' || *prev == ')')
            prev--;
        while (p != expr + strlen(expr) - 1 && *next == '(' || *next == ')')
            next++;

        if (is_symbol(*p) && 
            (((p != expr) && is_symbol(*prev)) | 
            ((p != expr + strlen(expr) - 1) && is_symbol(*next)))){
            return false;
        }
        if (isalpha(*p) && 
            (((p != expr) && isalpha(*prev)) | 
            ((p != expr + strlen(expr) - 1) && isalpha(*next)))){
            return false;
        }
        paren_check(*p);
    }
    
    if (parentheses != 0)
        nested_correctly = false;
    
    return nested_correctly;
}
