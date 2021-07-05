// cubes
// Written 13/3/2017
// by Sean O'Shea


#include <stdio.h>
#include <math.h>

int main (void) {

   double pow(double x, double y);
   int cube = 0;
   int n = 1;

	printf ("Enter how many cubes: ");
	scanf ("%d", &cube);

   while (n <= cube) {


      int cubic = pow(n,3);

	   printf ("%d", n);
	   printf ("^3 = ");
	   printf ("%d\n", cubic);
	
	   n=n+1;

   }
   return 0;
   }

