// Stores the intervals needed to determine if the logic
// variables should be true or false. Returns the number
// of intervals.
int find_intervals(int intervals[26^2][2], int char_num, 
                   int num_lines, int cur_line);
// Checks if the current line is in an interval where the variable should be true.
bool check_intervals(int intervals[26^2][2], int cur_line, int num_intervals);
// Sets the variables in the logic expression to true or false based on line number.
// This is stored in char_vals. actv_vars tracks what variables are being used,
// while num_vars track how many variables there are.
void set_vals(bool *char_vals, int *actv_vars, int cur_line, int num_lines, int num_vars); 
