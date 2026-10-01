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
void test_eval_one(void);
void test_eval_two(void);
void test_storage(void);

bool run_tests(void){    
    puts("--------------------------------");
    printf("Running tests...\n"); 
    test_validation();
    test_eval_one();
    test_eval_two();
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
    *expr6 = "A|B&(A|(C|((D&B))", *expr7 = "A|B&(A|(C|)((D&B))))",
    *expr8 = "A & B", *expr9 = "A | B";
    bool passed, is_valid0 = is_valid(expr0), is_valid1 = is_valid(expr1),
    is_valid2 = is_valid(expr2), is_valid3 = is_valid(expr3), 
    is_valid4 = is_valid(expr4), is_valid5 = is_valid(expr5),
    is_valid6 = is_valid(expr6), is_valid7= is_valid(expr7),
    is_valid8 = is_valid(expr8), is_valid9 = is_valid(expr9);
    puts("\n--------------------------------");
    puts("--------------------------------");
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
    printf("9. is_valid: %s. Expected: True. Received: %s\n",
           expr8, T_F(is_valid8));
    printf("10. is_valid: %s. Expected: True. Received: %s\n",
           expr9, T_F(is_valid9));
    
    passed = !is_valid0 && is_valid1 && !is_valid2 && !is_valid3 && is_valid4 && is_valid5 && !is_valid6 && !is_valid7;
    
    if (!passed)
        puts("test_validation failed!");
    
    if (tests_passed)
            tests_passed = passed;
    
}

void test_eval_one(void){
    bool passed;
    char *expr0 = "P | Q | (P & ~(P | Q))", *expr1 = "A & B | C (A & ~D)", *expr2 = "~P & ~(P | (A | Q) | P & Q & ~Q | Z | (P & (A & Q))",
    *prep_expr0 = "1|0|(1&~(1|0))",
    *prep_expr1 = "1&0|0|(1&~1)", 
    *prep_expr2 = "~0&~(0|1|0)|1&0&~0|1|(1&(1&0))",
    *prep_expr3 = "1&0|0&1&0&1&0&1&0&1";
    
    puts("\n--------------------------------");
    puts("--------------------------------");   
    printf("Testing find_right_para...\n");
    char *find0 = find_right_para(expr0 + 15, expr0 + strlen(expr0));
    char *find1 = find_right_para(expr0 + 13, expr0 + strlen(expr0));
    bool is_valid_loc0 = find0 == (expr0 + strlen(expr0) - 2);
    bool is_valid_loc1 = find1 == (expr0 + strlen(expr0) - 2);
    printf("Testing from %ld... ", (long int) expr0 + 15); 
    printf("Settled on %c at %ld. Test Passed: %s\n",
            *find0, (long int) find0, T_F(is_valid_loc0));
    
    printf("Testing from %ld... ", (long int) expr0 + 13); 
    printf("Settled on %c at %ld. Test Passed: %s\n",
            *find1, (long int) find1, T_F(is_valid_loc1));
    
    puts("--------------------------------");
    printf("Testing find_left_para...\n");
    char *find2 = find_left_para(expr0, expr0 + 15);
    char *find3 = find_left_para(expr0, expr2 + 20);
    bool is_valid_loc2 = find2 == expr0 + 14;
    bool is_valid_loc3 = find3 == expr2 + 6;
    
    printf("Testing from %ld... ", (long int)  (expr0 + 15)); 
    printf("Settled on %c at %ld. Test Passed: %s\n", *find2, (long) find2, T_F(is_valid_loc2));
    
    printf("Testing from %ld... ", (long int) expr2 + 20); 
    printf("Settled on %c at  %ld. Test Passed: %s\n", *find3, (long) find3, T_F(is_valid_loc3));
    
    puts("--------------------------------");
    printf("Testing find_next_var...\n");
    char *find4 = find_next_var(prep_expr0 + 1, prep_expr0 + strlen(expr0));
    char *find5 = find_next_var(prep_expr2, prep_expr2 + strlen(expr2) - 4);
    bool is_valid_loc4 = find4 == prep_expr0 + 2;
    bool is_valid_loc5 = find5 == prep_expr2 + 1;
    
    printf("Testing from %ld... ", (long int) prep_expr0 + 2); 
    printf("Settled on %c at %ld. Expected: True. Received: %s\n", 
           *find4, (long int) find4, T_F(is_valid_loc4));
    
    printf("Testing from %ld...\n", (long int) prep_expr2); 
    printf("Settled on %c at %ld. Expected: True. Received: %s\n",
           *find5, (long int) find5, T_F(is_valid_loc5));
    
    puts("--------------------------------");
   
    
    passed = is_valid_loc0 && is_valid_loc1 && is_valid_loc2 &&
        is_valid_loc3 && is_valid_loc4 && is_valid_loc5;
    
    if (!passed)
        puts("test_eval_one failed!"); 
    
    if (tests_passed)
        tests_passed = passed;
    
}

void test_eval_two(void){
    bool char_vals[26] = {false};
    char_vals['A' - 65] = true;
    char_vals['P' - 65] = true;
    char_vals['D' - 65] = true;
    char_vals['Z' - 65] = true;
    char *next_val, 
    prep_expr0[] = "1",
    prep_expr1[] = "0",
    prep_expr2[] = "~1",
    prep_expr3[] = "~0", 
    prep_expr4[] = "~1&0&0|1|1|~1&~0|~1", 
    prep_expr5[] = "~0&~(0|1|0)|1&0&~0|1|(1&(1&0))",
    prep_expr6[] = "1&0|0&1&0&1&0&1&0&1";
    
    puts("\n--------------------------------");
    puts("--------------------------------");
    printf("\nTesting find_next_var...\n");
    
    printf("Expecting 1, found %s\n", find_next_var(prep_expr2, prep_expr2 + 1));
    printf("Expecting 0, found %s\n", find_next_var(prep_expr3, prep_expr3 + 1));
    
    puts("--------------------------------");
    printf("Testing eval_not...\n");
    printf("%s, %s, %s, %s, and (%s) became:\n", prep_expr0, prep_expr1, prep_expr2, prep_expr3, prep_expr4); 
    eval_not(prep_expr0, prep_expr0 + strlen(prep_expr0), next_val);
    eval_not(prep_expr1, prep_expr1 + strlen(prep_expr1), next_val);
    eval_not(prep_expr2, prep_expr2 + strlen(prep_expr2), next_val);
    eval_not(prep_expr3, prep_expr3 + strlen(prep_expr3), next_val);
    eval_not(prep_expr4, prep_expr4 + strlen(prep_expr4), next_val);
    
    printf("%s, %s, %s, %s, and (%s)\n", prep_expr0, prep_expr1, prep_expr2, prep_expr3, prep_expr4); 
    
    puts("--------------------------------");
    printf("Testing eval_and...\n");  
    
    printf("%s, %s, %s, %s, and (%s) became:\n", prep_expr0, prep_expr1, prep_expr2, prep_expr3, prep_expr4); 
    eval_and(prep_expr0, prep_expr0 + strlen(prep_expr0), next_val);
    eval_and(prep_expr1, prep_expr1 + strlen(prep_expr1), next_val);
    eval_and(prep_expr2, prep_expr2 + strlen(prep_expr2), next_val);
    eval_and(prep_expr3, prep_expr3 + strlen(prep_expr3), next_val);
    eval_and(prep_expr4, prep_expr4 + strlen(prep_expr4), next_val);
    printf("%s, %s, %s, %s, and (%s)\n", prep_expr0, prep_expr1, prep_expr2, prep_expr3, prep_expr4); 
    
    
    puts("--------------------------------");
    printf("Testing eval_or...\n");  
    printf("%s, %s, %s, %s, and (%s) became:\n", prep_expr0, prep_expr1, prep_expr2, prep_expr3, prep_expr4); 
    eval_or(prep_expr0, prep_expr0 + strlen(prep_expr0), next_val);
    eval_or(prep_expr1, prep_expr1 + strlen(prep_expr1), next_val);
    eval_or(prep_expr2, prep_expr2 + strlen(prep_expr2), next_val);
    eval_or(prep_expr3, prep_expr3 + strlen(prep_expr3), next_val);
    eval_or(prep_expr4, prep_expr4 + strlen(prep_expr4), next_val);
    printf("%s, %s, %s, %s, and (%s)\n", prep_expr0, prep_expr1, prep_expr2, prep_expr3, prep_expr4); 
    
    puts("--------------------------------");
    printf("Testing eval_paren...\n");
    
    printf("%s \nbecame: \n", prep_expr5); 
    eval_paren(prep_expr5, prep_expr5 + strlen(prep_expr5), next_val);
    puts(prep_expr5);
    puts("--------------------------------");
    printf("Testing eval_expr...\n");
    strcpy(prep_expr4, "~1&0&0|1|1|~1&~0|~1"); 
    strcpy(prep_expr5, "~0&~(0|1|0)|1&0&~0|1|(1&(1&0))");
    strcpy(prep_expr6, "1&0|0&1&0&1&0&1&0&1");
    
    printf("\n\t[%s],\n \t[%s],\n \t[%s]\nBecame:\n", prep_expr4, prep_expr5, prep_expr6); 
    eval_expr(prep_expr4, prep_expr4 + strlen(prep_expr4));
    eval_expr(prep_expr5, prep_expr5 + strlen(prep_expr5));
    eval_expr(prep_expr6, prep_expr6 + strlen(prep_expr6));
    printf("\n\t[%s],\n \t[%s],\n \t[%s]\n", prep_expr4, prep_expr5, prep_expr6); 
    
    
   puts("--------------------------------");
    printf("Testing run_expr...\n");
    char expr4[] = "~P&~(P|(A|Q))|P&Q&~Q|Z|(P&(A&Q))";
    printf("\t%s\n was evaluated as %s", expr4, T_F(run_expr(expr4, char_vals, strlen(expr4), 4)));
}

void test_storage(void){
    bool passed;
    int num_expr = 2;
    char *expr0 = "this is an expression", *expr1 = "A & B | C (A & ~D)", 
    *expr2 = "z", *expr3 = "Elephant",
    *expr4 = "~P & ~(P | (A | Q)) | P & Q & ~Q | Z | (P & (A & Q))",
    *expr5 = "F| G",
    *expressions0[MAX_EXPR] = {expr0, expr1}, *expressions1[MAX_EXPR], 
    *expressions2[MAX_EXPR];
    
    puts("\n--------------------------------");
    puts("--------------------------------");
    puts("Testing get_next()...");
    printf("Next on expr4 + 5 is %s\n", get_next(expr4 + 5, expr4 + strlen(expr4)));
    
    printf("Testing contains_expr...\n");
    bool contains_expr0 = contains_expr(expressions0, MAX_EXPR, expr0,
                                        expr0 + strlen(expr0) + 1, &num_expr),
    contains_expr1 = contains_expr(expressions0, MAX_EXPR, expr1,
                                   expr1 + strlen(expr1) + 1, &num_expr),
    contains_expr3 = contains_expr(expressions0, MAX_EXPR, expr3,
                                   expr3 + strlen(expr3) + 1, &num_expr),
    contains_expr4 = contains_expr(expressions0, MAX_EXPR, expr4,
                                   expr4 + strlen(expr4) + 1, &num_expr),    
    contains_expr5 = contains_expr(expressions0, MAX_EXPR, expr5,
                                   expr5 + strlen(expr5) + 1, &num_expr);    
    printf("contains_expr: %s Expected: True. Received %s\n",
           expr0, T_F(contains_expr0));  
    printf("contains_expr: %s Expected: True. Received %s\n",
           expr1, T_F(contains_expr1));  
    printf("contains_expr: %s Expected: False. Received %s\n",
           expr3, T_F(contains_expr3)); 
    printf("contains_expr: %s Expected: False. Received %s\n",
           expr4, T_F(contains_expr4)); 
    printf("contains_expr: %s Expected: False. Received %s\n",
           expr5, T_F(contains_expr5)); 
    
    printf("Testing add_expr...\n");        
    num_expr = 0;
    add_expr(expressions1, MAX_EXPR, expr0, expr0 + strlen(expr0) + 1, &num_expr);
    add_expr(expressions1, MAX_EXPR, expr1, expr1 + strlen(expr1) + 1, &num_expr);
    add_expr(expressions1, MAX_EXPR, expr1, expr1 + strlen(expr1) + 1, &num_expr);
    add_expr(expressions1, MAX_EXPR, expr2, expr2 + strlen(expr2) + 1, &num_expr);
    
    printf("Expressions should have these expressions:\n\t%s\n\t%s\n\t%s\n",
           expr0, expr1, expr2);
    printf("It has: \n");
    for (int i = 0; i < num_expr; i++)
        printf("\t%s\n", expressions1[i]);
    for (int i = 0; i < num_expr; i++)
        free(expressions1[i]);     

    printf("Testing store_expressions with [%s]...\n", expr4);
    num_expr = 0;
    char *p = "p" ; 
    int char_count = store_expressions(expressions2, MAX_EXPR, expr4, expr4 + strlen(expr4) + 1, &num_expr, 0);
    
    printf("Expressions1 should have these expressions:\n\t%s\n\t%s\n\t%s\n",
           "fill in", "", "");
    printf("There are %d expressions:\n", num_expr); 
    for (int i = 0; i < num_expr; i++)
        printf("\t%s\n", expressions2[i]);
    
    for (int i = 0; i < num_expr; i++)
        free(expressions2[i]);      
    
    passed = contains_expr0 && contains_expr1 && !contains_expr3 && !contains_expr4 && !contains_expr5; 
    
    
    if (!passed)
        puts("test_storage failed!");
    
    if (tests_passed)
        tests_passed = passed;
}
