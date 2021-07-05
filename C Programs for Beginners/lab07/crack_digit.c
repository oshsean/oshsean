// crack digit
// 19/4/17
// Sean O'Shea

#include <stdio.h>
#include "captcha.h"

void analyze_image(int height, int width, int pixels[height][width]);
int identify_digit(double horizontal_balance, int circles);

int main(int argc, char *argv[]) {
    int height, width;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image-file>\n", argv[0]);
        return 1;
    }

    if (get_pbm_dimensions(argv[1], &height, &width) != 1) {
        return 1;
    }

    int pixels[height][width];
    if (read_pbm(argv[1], height, width, pixels)) {
        analyze_image(height, width, pixels);
    }
    return 0;
}


void analyze_image(int height, int width, int pixels[height][width]) {
    int start_row, start_column, box_width, box_height;

    get_bounding_box(height, width, pixels, &start_row, &start_column, &box_height, &box_width);

    int box_pixels[box_height][box_width];
    copy_pixels(height, width, pixels, start_row, start_column, box_height, box_width, box_pixels);

	double circles = find_circles(height, width, pixels[height][width]);

    double horizontal_balance = get_horizontal_balance(box_height, box_width, box_pixels);

    int digit = identify_digit(horizontal_balance);

    printf("%c\n", digit);
}

int identify_digit(double horizontal_balance, double circles) {
	if (circles = 1) {
		return '8'   
	} 
	else if (horizontal_balance < 0.5) {
        return '6';
    } else {
        return '3';
    }
}
