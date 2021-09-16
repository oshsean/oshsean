//letter_frequency
//4/4/17
//By Sean O'Shea
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void frequency(int number[], char c);
void print_frequency(int number[], double count);

int main(int argc, char *argv[]){
	char c = 0;
	int number[26] = {0};
	int count = 0;	

	while (c != EOF){
		c = getchar();
		if (c != EOF){
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z' )){
				count = count + 1;
			}
			frequency(number, c);
		}
	}
	print_frequency(number, count);
	return 0;
}

void frequency(int number[], char c){
	if ('a' <= c &&  c <= 'z'){
		number[c - 'a'] = number[c - 'a'] + 1;
	}
	if ('A' <= c &&  c <= 'Z'){
		number[c - 'A'] = number[c - 'A'] + 1;
	}
}
void print_frequency(int number[], double count){
	int i = 0;
	double p = 0;
	while (i < 26){
		p = (number[i]/count);
		printf ("'%c' %lf %d \n", (i + 'a'), p, number[i]);
		i = i + 1;
	}
}

		
