// sum_cubes
// Written 14/3/2017
// by Sean O'Shea


#include <stdio.h>
#include <math.h>

int main (void) {

   double pow(double x, double y);
   int cubes = 0;
   int n = 1;
   int a= 0;

	printf ("Sum how many cubes? ");
	scanf ("%d", &cubes);
  
   
   while (n < cubes) {

	   printf ("%d^3 + ", n);
	
	   int c = pow(n,3);
 
      a=a+c;

	   n=n+1;
   }
   while (n == cubes) {

	   printf ("%d^3 = ", n);
	
	   int c = pow(n,3);
	
	   a=a+c;
	
	   n=n+1;
   }


   printf ("%d\n", a);
   
   return 0;
   }

