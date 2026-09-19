
void add_expr(char **expressions, int max_expr, char *expr, char *end, int *num_expr);
bool contains_var(char **expressions, int max_expr, char c);
bool contains_expr(char **expressions, int max_expr, char *expr, char *end);
int store_expressions(char **expressions, int max_expr, char *expr, char *end, int *num_expr);
