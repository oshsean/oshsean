// dating_range
// Written 7/3/2017
// by Sean O'Shea


#include <stdio.h>
#include <stdlib.h>

int main (void) {

int x = 0;

 printf ("Enter your age:");
 scanf ("%d", &x);
 
if (x < 12) {
	printf ("You are too young to be dating.\n");
}

else{

printf (" Your dating range is");

int (lower)= (x/2)+7;

printf (" %d", lower);
printf (" to");

int (higher)= (2*x)-14;

printf (" %d", higher);
printf (" years old.\n");
}
   return 0;

}
