// three_five
// Written 21/3/2017
// by Sean O'Shea


#include <stdio.h>

int main(void){
	int x = 0;
	int a = 1;

	printf ("Enter number: ");
	scanf ("%d", &x);
	
	while (a < x){

		if  (a % 3 == 0 || a % 5 == 0){
		printf ("%d\n", a);
		}
	a = a + 1;
	}
	


   return 0;

}
