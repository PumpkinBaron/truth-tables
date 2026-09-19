#include <stdbool.h>

bool check_intervals(int intervals[26^2][2], int cur_line, int num_intervals){
    for (int i = 0; i < num_intervals; i++){
        if (cur_line > intervals[i][0] && cur_line <= intervals[i][1])
            return true;
    }
    return false;
}

int find_intervals(int intervals[26^2][2], int char_num, 
                   int num_lines, int cur_line){
    int num_intervals = 2^(char_num - 1);
    int inc = num_lines / 2^char_num;
    
    for (int i = 0; i < num_intervals; i++){
        intervals[i][0] = ((num_lines * (i - 1)) / 2^char_num);
        intervals[i][1] = inc + intervals[i][0];
    }
    return num_intervals;
}

void set_vals(bool *char_vals, int *actv_vars, 
              int cur_line, int num_lines, int num_vars){
    int intervals[26^2][2];
    
    for (int i = 0; i < num_vars; i++){
        int num_intervals = find_intervals(intervals, i + 1, num_lines, cur_line);
        if (check_intervals(intervals, cur_line, num_intervals))
            char_vals[actv_vars[i]] = true;
        else
            char_vals[actv_vars[i]] = false;
    }
}
