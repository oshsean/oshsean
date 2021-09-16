//devowel programs
//1/4/17
//By Sean O'Shea

#include <stdio.h>

int main(void){

	int c = 0;

	c = getchar();
	while (c != EOF){
		if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u'){
			printf ("%c", c);
		}
		c = getchar();
	}
}
 
