#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "validate.h"
#include "eval.h"
#include "test.h"
#include "storage.h"
#include "intervals.h"

#define TEST_ON 0
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
    int num_vars = 0, actv_vars[26];
    bool char_vals[26] = {false};
    
    // Error checks
    if (argc != 2){
        printf("Correct usage: %s [expression], where the expression has no spaces.\n",
               argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if (TEST_ON)
        run_tests();
   
    for (char *p = argv[1]; *p != '\0'; p++){
        if (*p == '<' && *(p + 1) == '-' && *(p + 2) == '>'){
            *(p + 1) = ' ';
            *(p + 2) = ' ';
        } else if (*p == '-' && *(p + 1) == '>')
            *p = ' ';
    }
    puts(argv[1]);
    
    is_valid(argv[1]);
    
    // Register variables
    // Initialize the array to a number not associated with any variable.
    for (int i = 0; i < 26; i++)
        actv_vars[i] = -500;
    
    for (char *p = argv[1]; *p != '\0'; p++){
        if (isalpha(*p) && !(is_actv(actv_vars, *p - 65))){  
            actv_vars[num_vars++] = toupper(*p) - 65;
        }
    }
    
    qsort(actv_vars, num_vars, sizeof(int), cmpr_vars); 
    
    
    // Print truth table
    print_table(argv[1], char_vals, actv_vars, num_vars, (int) pow(2, num_vars + 1));
    
    return 0;
}


void print_line(char *expression[MAX_EXPR], int num_expr){
    putchar(' ');
    for (int i = 0; i < num_expr; i++){
        printf("---");
        for (char *p = expression[i]; *p != '\0'; p++)
            putchar('-');
    } 
    printf("-\n");
}


void print_table(char *expr, bool *char_vals, int *actv_vars, int num_vars, int num_lines){
    char *expressions[MAX_EXPR];
    int num_expr = 0;
    store_expressions(expressions, MAX_EXPR, expr, expr + strlen(expr), &num_expr, 0);
    
    qsort(expressions, num_expr, sizeof(char *), cmpr_expr);
     
    
    for (int i = 0; i < num_expr; i++)
        puts(expressions[i]);
    print_line(expressions, num_expr);
    printf(" |");
    
    for (int i = 0; i < num_expr; i++){
        printf(" %s |", expressions[i]);
    }
    putchar('\n'); 
    for (int i = 0; i < num_lines; i++){  
        set_vals(char_vals, actv_vars, i, num_lines, num_vars);
        putchar(' ');
        for (int j = 0; j < num_expr; j++){ 
            int len = strlen(expressions[j]);
            printf("| ");
            if (run_expr(expressions[j], char_vals, len, num_vars))
                putchar('T');
            else 
                putchar('F');
            for (int k = 0; k < len; k++)
                putchar(' ');
        }
        printf("|\n");
    }
    print_line(expressions, num_expr);
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
    char *str_one = *(char **) a;
    char *str_two = *(char **) b; 
    if (strlen(str_one) == strlen(str_two))
        return strcmp(str_one, str_two);
    return strlen(str_one) - strlen(str_two) ;
}
