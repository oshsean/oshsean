// perfect
// Written 21/3/2017
// by Sean O'Shea


#include <stdio.h>

int main(void){
	int x = 0;
	int a = 1;
	int y = 0;


	printf ("Enter number: ");
	scanf ("%d", &x);
	
	printf ("The factors of %d are: \n", x);

	while (a <= x){

		if  (x%a==0){
		printf ("%d\n", a);
		y =y+a;
		}
	
	a=a+1;

	}
	
	printf ("Sum of factors = %d\n", y);
	
	if (y/2 == x){
	printf ("%d is a perfect number\n", x);
	}
	else if (y/2 != x){
	printf ("%d is not a perfect number\n", x);

	}
return 0;

}
