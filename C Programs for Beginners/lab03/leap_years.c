// leap_years
// Written 14/3/2017
// by Sean O'Shea


#include <stdio.h>
#include <stdlib.h>

int main (void) {

   int a = 0;
   int b = 0;

	printf ("Enter start year: ");
	scanf ("%d", &a);
	printf ("Enter finish year: ");
	scanf ("%d", &b);
	printf ("The leap years between %d and %d are: ", a, b);
   int c=a;

   while (c <= b){
      if (c%4!=0) {
	      c=c+1;
      } else if (c%100 == 0 && c%400!=0) {
	      c=c+1;

      } else {
	      printf ("%d ", c);
	      c=c+1;
      }
 
   }
   printf ("\n");

   return 0;

   }
