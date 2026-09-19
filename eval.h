// Replaces variables in a logic expression with 1 or 0, 
// corresponding to true and false, based on the values 
// in char_vals. It then evaluates with eval_expr().
bool run_expr(char *expr, bool *char_vals, int len, int num_vars); 
// Finds next right parenthesis
char * find_right_para(char *expr, char *end);
// Finds previous left parenthesis
char * find_left_para(char *expr, char *cur);
// Finds the earliest AND statements that's not in a higher 
// level of parenthesis'
char *find_and_start(char *expr, char *cur);
char *find_next_var(char *start, char *end);
