// print_image
// 11/4/17
// Sean O'Shea

#include <stdio.h>
#include "captcha.h"

void print_image(int height, int width, int pixels[height][width]){

	int currenth = height - 1;
  	int currentw = 0;


	while (currenth >= 0){
		currentw = 0;
		while (currentw < width){
			if (pixels[currenth][currentw] == 0){
				printf(".");				
			}
			else if (pixels[currenth][currentw] == 1){
				printf("*");
			}
			currentw++;
		}
		printf ("\n");
		currenth = currenth - 1;
	}
	
}

