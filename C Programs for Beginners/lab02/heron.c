// heron
// Written 7/3/2017
// by Sean O'Shea


#include <stdio.h>
#include <math.h>

int main (void) {

double a =0;
double b =0;
double c =0;

 printf ("Enter side 1: ");
 scanf (" %lf", &a);
 printf	("Enter side 2: ");
 scanf (" %lf", &b);
 printf	("Enter side 3: ");
 scanf (" %lf", &c);
 if ( a + b < c || a + c < b || b + c < a){ 
   printf ("Error: triangle inequality violated.\n");}
 else{
 double s =(a+b+c)/2;
 double Area = sqrt(s*(s-a)*(s-b)*(s-c));
	printf("Area= %lf\n", Area);}

   return 0;

}
