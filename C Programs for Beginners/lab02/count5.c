// count5
// Written 7/3/2017
// by Sean O'Shea


#include <stdio.h>
#include <math.h>

int main (void) {

double a=0;

 printf ("please enter an integer:");
 scanf ("%lf", &a);
 if ( a < 1 ) {
	printf (" You entered a number less than one.\n");}
 else if (a == 5){ 
	printf (" You entered five.\n");}
 else if (a == 4){ 
	printf (" You entered four.\n");}
 else if (a == 3){ 
	printf (" You entered three.\n");}
 else if (a == 2){ 	
	printf (" You entered two.\n");}
 else if (a == 1){ 
	printf (" You entered one.\n");}
 else { 
	printf (" You entered a number greater than five.\n");
 }

   return 0;

}
