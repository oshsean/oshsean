// is_leap_year
// Written 13/3/2017
// by Sean O'Shea


#include <stdio.h>
#include <stdlib.h>

int main (void) {

   int Year = 0;

	printf ("Enter Year: ");
	scanf ("%d", &Year);

   if (Year%4!=0) {
	   printf ("%d", Year);
	   printf (" is not a leap year.\n");
   } else if (Year%100==0 && Year%400!=0) {
	   printf ("%d", Year);
	   printf (" is not a leap year.\n");

   } else {
	   printf ("%d", Year);
	   printf (" is a leap year.\n");	
   }

   return 0;

   }
