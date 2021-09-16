// bounding box
// 11/4/17
// Sean O'Shea

#include <stdio.h>
#include "captcha.h"

void get_bounding_box(int height, int width, int pixels[height][width], int *start_row, int *start_column, int *box_height, int *box_width) {
    int min_row = height - 1;
    int max_row = 0;
    int min_col = width - 1;
    int max_col = 0;
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){
            if (pixels[row][col] == 1){
                if (row < min_row){
                    min_row = row;
                }
                if (col < min_col){
                    min_col = col;
                }
                if (row > max_row){
                    max_row = row;
                }
                if (col > max_col){
                    max_col = col;
                }
            }
        }
    }
    if (min_row <= max_row){
        *start_row = min_row;
        *box_height = max_row -  min_row + 1;
        *start_column = min_col;
        *box_width = max_col -  min_col + 1;
    } else{
        *start_row = 0;
        *box_height = 0;
        *start_column = 0;
        *box_width = 0;
    }
}
