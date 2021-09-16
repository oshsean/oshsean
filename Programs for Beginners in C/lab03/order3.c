// order3
// Written 13/3/2017
// by Sean O'Shea


#include <stdio.h>
#include <stdlib.h>

int main (void) {

   int a = 0;
   int b = 0;
   int c = 0;


	printf ("Enter integer: ");
	scanf ("%d", &a);
	printf ("Enter integer: ");
	scanf ("%d", &b);
	printf ("Enter integer: ");
	scanf ("%d", &c);
	printf ("The integers in order are: ");

   if ( c>b && b>a ){ 
	   printf ("%d ", a);
	   printf ("%d ", b);
	   printf ("%d\n", c);

   }
   else if ( b>c && c>a ){ 
	   printf ("%d ", a);
	   printf ("%d ", c);
	   printf ("%d\n", b);
   }
   else if ( c>a && a>b ){ 
	   printf ("%d ", b);
	   printf ("%d ", a);
	   printf ("%d\n", c);
   }
   else if ( a>c && c>b ){ 
	   printf ("%d ", b);
	   printf ("%d ", c);
	   printf ("%d\n", a);
   }
   else if ( b>a && a>c ){ 
	   printf ("%d ", c);
	   printf ("%d ", a);
	   printf ("%d\n", b);
   }

   else if ( a>b && b>c )
   { 
	   printf ("%d ", c);
	   printf ("%d ", b);
	   printf ("%d\n", a);
   }
   else if ( a==b && b>c )
   { 
	   printf ("%d ", c);
	   printf ("%d ", b);
	   printf ("%d\n", a);
   }
   else if ( a==c && c>b )
   { 
	   printf ("%d ", b);
	   printf ("%d ", c);
	   printf ("%d\n", a);
   }
   else if ( b==c && b>a )
   { 
	   printf ("%d ", a);
	   printf ("%d ", b);
	   printf ("%d\n", c);
   }
   else if ( a==b && c>b )
   { 
	   printf ("%d ", a);
	   printf ("%d ", b);
	   printf ("%d\n", c);
   }
   else if ( a==c && b>c )
   { 
	   printf ("%d ", a);
	   printf ("%d ", c);
	   printf ("%d\n", b);
   }
   else if ( b==c && a>b )
   { 
	   printf ("%d ", c);
	   printf ("%d ", b);
	   printf ("%d\n", a);
   }
   else if ( b==c && a==b )
   { 
	   printf ("%d ", c);
	   printf ("%d ", b);
	   printf ("%d\n", a);
   }

   return 0;
   }
