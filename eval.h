// Replaces variables in a logic expression with 1 or 0, 
// corresponding to true and false, based on the values 
// in char_vals. It then evaluates with eval_expr().
bool run_expr(char *expr, bool *char_vals, int len, int num_vars); 
// Finds next right parenthesis
char * find_right_para(char *expr, char *end);
// Finds previous left parenthesis
char * find_left_para(char *expr, char *cur);
// Finds the earliest AND statement that's not in a higher 
// level of parentheses. It stops on the &. 
char *find_next_var(char *start, char *end);
// included for testing
bool eval_expr(char *expr, char *end);
void eval_paren(char *expr, char *end, char *next_val);
void eval_not(char *expr, char *end, char *next_val);
void eval_or(char *expr, char *end, char *next_val);
void eval_and(char *expr, char *end, char *next_val);

char *find_next_var(char *start, char *end);
