// copy pixels
// 18/4/17
// Sean O'Shea

#include <stdio.h>
#include "captcha.h"


void copy_pixels(int height, int width, int pixels[height][width], int start_row, int start_column, int copy_height, int copy_width, int copy[copy_height][copy_width]) {
    for (int row = 0; row < copy_height; row++) {
        for (int column = 0; column < copy_width; column++) {
            copy[row][column] = pixels[start_row + row][start_column + column];
        }
    }
}
