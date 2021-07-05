// border
// 11/4/17
// Sean O'Shea

#include <stdio.h>
#include "captcha.h"

void print_image(int height, int width, int pixels[height][width]);
int main(void){

	int currenth = 0;
  	int currentw = 0;
	int width = 0;
	int height = 0;
	int pixels [100][100];

	while (currenth < height){
		while (currentw < width){
			if (pixels == 0){
				printf(".");				
			}
			else if (pixels == 1){
				printf("*")
			}
			currentw++;
		}
		currenth++;
	}
	
}
