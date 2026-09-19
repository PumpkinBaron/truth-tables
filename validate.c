#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LENGTH 1026

bool nested_correctly = true, prev_was_symb = false,
     consec_symb = false, prev_was_var = false, consec_var = false;
int parentheses = 0; // left only
char contents[MAX_LENGTH];

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
        case '~':
            is_symb = true;
            break;
    }
    return is_symb;
}

bool is_valid(char *expr){
    char c = '\0'; 
    
    for (int i = 0; i < MAX_LENGTH && contents[i] != '\0' && nested_correctly; i++){
        paren_check(contents[i]);
        
        if (is_symbol(contents[i])){
            if (prev_was_symb)
                consec_symb = true;
        }
        else
            prev_was_symb = false;
        
        if (isalpha(contents[i])){
            if (prev_was_var)
                consec_var = true;
        }
        else if (contents[i] != ' ')
            prev_was_var = false;
    }
        
    return nested_correctly && !consec_symb && !consec_var;
}
