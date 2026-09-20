#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "test.h"
#include "eval.h" 
#include "storage.h"
#include "intervals.h"
#include "validate.h"

#define T_F(x) (x ? "True" : "False")
#define MAX_EXPR 1000

bool tests_passed = true;

void test_validation(void);
void test_eval(void);
void test_storage(void);

bool run_tests(void){    
    printf("Running tests... "); 
    test_validation();
    test_eval();
    test_storage();
    
    if (tests_passed)
        printf("Tests successful! Starting program...\n");
    else {
        printf("Tests failed!\n");
        exit(EXIT_FAILURE);        
    }
    
    
    return tests_passed;
    
    
}

void test_validation(void){
    char *expr0 = "hello", *expr1 = "B&~~~~C", *expr2 = "B&&C",
    *expr3 = "A&B&C||D", *expr4 = "A&B&C|D", *expr5 = "A|B&(A|(C|(D&B)))",
    *expr6 = "A|B&(A|(C|((D&B))", *expr7 = "A|B&(A|(C|)((D&B))))";
    bool is_valid0 = is_valid(expr0), is_valid1 = is_valid(expr1),
    is_valid2 = is_valid(expr2), is_valid3 = is_valid(expr3), 
    is_valid4 = is_valid(expr4), is_valid5 = is_valid(expr5),
    is_valid6 = is_valid(expr6), is_valid7= is_valid(expr7);
    printf("Testing is_valid...\n");
         
    printf("1. is_valid: %s. Expected: False. Received: %s\n",
           expr0, T_F(is_valid0));
    printf("2. is_valid: %s. Expected: True. Received: %s\n",
           expr1, T_F(is_valid1));
    printf("3. is_valid: %s. Expected: False. Received: %s\n",
           expr2, T_F(is_valid2));
    printf("4. is_valid: %s. Expected: False. Received: %s\n",
           expr3, T_F(is_valid3));
    printf("5. is_valid: %s. Expected: True. Received: %s\n",
           expr4, T_F(is_valid4));
    printf("6. is_valid: %s. Expected: True. Received: %s\n",
           expr5, T_F(is_valid5));
    printf("7. is_valid: %s. Expected: False. Received: %s\n",
           expr6, T_F(is_valid6));
    printf("8. is_valid: %s. Expected: False. Received: %s\n",
           expr7, T_F(is_valid7));
    
    if (tests_passed)
        tests_passed = !is_valid0 && is_valid1 && !is_valid2 && !is_valid3 && is_valid4 && is_valid5 && !is_valid6 && !is_valid7;
    
}

void test_eval(void){
    char *expr0 = "P | Q | (P & ~(P | Q))", *expr1 = "A & B | C (A & ~D)", *expr2 = "~P & ~(P | (A | Q) | P & Q & ~Q | Z | (P & (A & Q))",
    *prep_expr0 = "1 | 0 | (1 & ~(1 | 0))",
    *prep_expr1 = "1 & 9 | 0 (1 & ~1)", 
    *prep_expr2 = "~0 & ~(0 | 1 | 0) | 1 & 0 & ~0 | 1 | (1 & (1 & 0))";
    bool char_vals[26] = {false};
    char_vals['A' - 65] = true;
    char_vals['P' - 65] = true;
    char_vals['D' - 65] = true;
    char_vals['Z' - 65] = true;
    
    printf("Testing find_right_para...\n");
    char *find0 = find_right_para(expr0 + 15, expr0 + strlen(expr0));
    char *find1 = find_right_para(expr0 + 15, expr0 + strlen(expr0));
    bool is_valid_loc0 = find0 == expr0 + strlen(expr0);
    bool is_valid_loc1 = find1 == expr0 + strlen(expr0) - 1;
    bool is_right_para0 = *find0 == ')', is_right_para1 = *find1 == ')';
    
    printf("Testing from %c... ", expr0[15]); 
    printf("Settled on %c. Expected: True. Received: %s\n",
           *find0, T_F(is_valid_loc0));
    
    printf("Testing find_right_para from %c...\n", expr0[21]); 
    printf("Settled on %c. Expected: True. Received: %s\n",
           *find1, T_F(is_valid_loc1));
    
    printf("Testing find_left_para...\n");
    char *find2 = find_left_para(expr0 + 15, expr0 + strlen(expr0));
    char *find3 = find_left_para(expr0 + strlen(expr0), expr0 + 15);
    bool is_valid_loc2 = find2 == expr0 + strlen(expr0);
    bool is_valid_loc3 = find3 == expr0 + strlen(expr0) - 1;
    bool is_left_para0 = *find0 == '(', is_left_para1 = *find1 == '(';
    
    printf("Testing from %c... ", *(expr0 + 15)); 
    printf("Settled on %c. Expected: True. Received: %s\n",
           *find2, T_F(is_valid_loc2));
    
    printf("Testing find_right_para from %c...\n", *(expr0 + strlen(expr0))); 
    printf("Settled on %c. Expected: True. Received: %s\n",
           *find3, T_F(is_valid_loc3));
    
    printf("Testing find_next_var...\n");
    char *find4 = find_next_var(prep_expr0 + 15, prep_expr0 + strlen(expr0));
    char *find5 = find_next_var(prep_expr2, prep_expr2 + strlen(expr2));
    bool is_valid_loc4 = find4 == prep_expr0 + 21;
    bool is_valid_loc5 = find5 == prep_expr2;
    bool is_var0 = (*find4 == '0') | (*find4 == '1'), 
    is_var1 = (*find5 == '0') | (*find5 == '1');
    
    printf("Testing from %c... ", *(prep_expr0 + 15)); 
    printf("Settled on %c. Expected: True. Received: %s\n",
           *find4, T_F(is_valid_loc4));
    
    printf("Testing find_right_para from %c...\n", *prep_expr2); 
    printf("Settled on %c. Expected: True. Received: %s\n",
           *find5, T_F(is_valid_loc5));
    
    printf("Testing find_and_start...\n");
    char *find6 = find_and_start(prep_expr2, 
                                 prep_expr2 + strlen(prep_expr2));
    char *find7 = find_and_start(prep_expr1, prep_expr1 + 5);
    bool is_valid_loc6 = find6 == expr0 + 23;
    bool is_valid_loc7 = find7 == expr0 + 3;
    bool is_and0 = *find6 == '&', is_and1 = *find1 == '&';
    
    printf("Testing from %c... ", *(prep_expr2 + strlen(prep_expr2))); 
    printf("Settled on %c. Expected: True. Received: %s\n",
           *find6, T_F(is_valid_loc6));
    
    printf("Testing find_right_para from %c...\n", *(prep_expr1 + 5)); 
    printf("Settled on %c. Expected: True. Received: %s\n",
           *find7, T_F(is_valid_loc7));
    
    printf("Testing run_expr...\n");
    char expr4[] = "~P & ~(P | (A | Q) | P & Q & ~Q | Z | (P & (A & Q))";
    printf("Expression was [%s] and now is ", expr4);
    run_expr(expr4, char_vals, strlen(expr4), 4);
    printf("[%s].\n", expr4);
    
    if (tests_passed) 
        tests_passed = is_valid_loc0 && is_valid_loc1 && is_valid_loc2 &&
        is_valid_loc3 && is_valid_loc4 && is_valid_loc5 && is_valid_loc6 &&
        is_valid_loc7 && is_right_para0 && is_right_para1 && is_left_para0 &&
        is_left_para1 && is_var0 && is_var1 & is_and0 && is_and1;
    
}

void test_storage(void){
    int num_expr = 0;
    char *expressions0[MAX_EXPR], expressions1[MAX_EXPR], 
    *expr0 = "this is an expression", *expr1 = "A & B | C (A & ~D)", 
    *expr2 = "z", *expr3 = "Elephant",
    *expr4 = "~P & ~(P | (A | Q) | P & Q & ~Q | Z | (P & (A & Q))";
    
    printf("Testing contains_expr...\n");
    bool contains_expr0 = contains_expr(expressions0, MAX_EXPR, expr0,
                                        expr0 + strlen(expr0) + 1, &num_expr),
    contains_expr1 = contains_expr(expressions0, MAX_EXPR, expr1,
                                   expr1 + strlen(expr1) + 1, &num_expr),
    contains_expr3 = contains_expr(expressions0, MAX_EXPR, expr3,
                                   expr3 + strlen(expr3) + 1, &num_expr);    
    printf("contains_expr: %s Expected: True. Received %s\n",
           expr0, T_F(contains_expr0));  
    printf("contains_expr: %s Expected: True. Received %s\n",
           expr1, T_F(contains_expr1));  
    printf("contains_expr: %s Expected: False. Received %s\n",
           expr3, T_F(contains_expr3)); 
    
    printf("Testing add_expr...\n");        
    add_expr(expressions0, MAX_EXPR, expr0, expr0 + strlen(expr0) + 1, &num_expr);
    add_expr(expressions0, MAX_EXPR, expr1, expr1 + strlen(expr1) + 1, &num_expr);
    add_expr(expressions0, MAX_EXPR, expr1, expr1 + strlen(expr1) + 1, &num_expr);
    add_expr(expressions0, MAX_EXPR, expr2, expr2 + strlen(expr2) + 1, &num_expr);
    
    printf("Expressions should have these expressions:\n\t%s\n\t%s\n\t%s\n",
           expr0, expr1, expr2);
    printf("It has: \n");
    for (int i = 0; i < num_expr; i++)
        printf("\t%s\n", expressions0[i]);     

    // This is dependent on test_eval passing.    
    //    printf("Testing store_expressions...\n");
    //    num_expr = 0;
    //    int char_count = store_expressions((char **) expressions1, MAX_EXPR, expr4, (char *) strlen(expr4) + 1, &num_expr, 0);
    
    //    printf("Expressions1 should have these expressions:\n\t%s\n\t%s\n\t%s\n",
    //           "fill in", "", "");
    //    printf("It has: \n");
    //    for (int i = 0; i < num_expr; i++)
    //        printf("%s\n", expressions0[i]);
    
    if (tests_passed){
        tests_passed = contains_expr0 && contains_expr1 && !contains_expr3;
    }
}
