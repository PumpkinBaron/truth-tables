#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "validate.h"
#include "eval.h"
#include "test.h"
#include "storage.h"
#include "intervals.h"

#define TEST_ON 1
#define MAX_EXPR 1000

// Tell quicksort to use alphabetic order 
int cmpr_vars(const void *a, const void *b);
// Tell quicksort to use shortest strings first. If they are the same
// length, it uses alphabetical order.
int cmpr_expr(const void *a, const void *b);
// Variables are represented by the letters A-Z by the number 0-25;
bool is_actv(int actv_vars[26], int var);
// Prints the truth table,
void print_table(char *expr, bool *char_vals, int *actv_vars, int num_vars, int num_lines);

int main(int argc, char * argv[]){
    int num_vars, actv_vars[26];
    bool char_vals[26];
    
    if (TEST_ON)
        run_tests();
    
    // Error checks
    if (argc != 2)
        printf("Correct usage: %s [expression], where the expression has no spaces.",
               argv[0]);
    
    is_valid(argv[1]);
    
    // Register variables
    for (char *p = argv[1]; *p != '\0'; p++){
        if (isalpha(*p) && !(is_actv(actv_vars, *p - 65))){
            num_vars++;
            actv_vars[num_vars] = *p - 65;
        }
    }
    qsort(actv_vars, num_vars * sizeof(int), sizeof(int), cmpr_vars);
    
    // Print truth table
    print_table(argv[1], char_vals, actv_vars, num_vars, 2^num_vars);
    
    return 0;
}

void print_table(char *expr, bool *char_vals, int *actv_vars, int num_vars, int num_lines){
    char *expressions[MAX_EXPR];
    int num_expr = num_vars, 
    char_count = store_expressions(expressions, MAX_EXPR, expr, expr + strlen(expr), &num_expr);
    qsort(expressions, sizeof(expressions) / sizeof(char *), sizeof(char *), cmpr_expr);
    
    putchar(' ');
    for (int i = 0; i < char_count; i++)
        putchar('-');
    putchar('\n');
    
    for (int i = 0; i < num_lines; i++){
        set_vals(char_vals, actv_vars, i, num_lines, num_vars);
        for (int j = 0; j < num_expr; j++){
            int len = strlen(expressions[j]);
            puts(" | ");
            if (run_expr(expressions[j], char_vals, len, num_vars))
                putchar('T');
            else 
                putchar('F');
            for (int k = 0; k < len + 1; k++)
                putchar(' ');
        }
    }
    putchar(' ');
    for (int i = 0; i < char_count; i++)
        putchar('-');
    putchar('\n');
}


bool is_actv(int actv_vars[26], int var){
    for (int i = 0; i < 26; i++)
        if (actv_vars[i] == var)
            return true;
    return false;
}

int cmpr_vars(const void *a, const void *b){
    return *(const int*) a - *(const int*) b;
}

int cmpr_expr(const void *a, const void *b){
    if (strlen(a) == strlen(b))
        return strcmp(a, b);
    return *(const int*) strlen(a) - *(const int*) strlen(b);
}
