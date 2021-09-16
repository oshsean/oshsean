// easter
// Written 7/3/2017
// by Sean O'Shea


#include <stdio.h>
#include <stdlib.h>

int main (void) {

int Year = 0;

printf ("Enter Year: ");
scanf ("%d", &Year);



int a=Year%19;
int b=Year/100;
int c=Year%100;
int d=b/4;
int e=b%4;
int f=(b+8)/25;
int g=(b-f+1)/3;
int h=(19*a+b-d-g+15)%30;
int i=c/4;
int k=c%4;
int l=(32+2*e+2*i-h-k)%7;
int m=(a+11*h+22*l)/451;
int EasterMonth =(h+l-7*m+114)/31; 
int p=(h+l-7*m+114)%31;
int EasterDate=p+1;

printf ("Easter is ");
if (EasterMonth == 3){
printf ("March ");
printf ("%d", EasterDate );
printf (" in %d.\n", Year );
}
else if (EasterMonth ==4){
printf ("April");
printf (" %d", EasterDate );
printf (" in %d.\n", Year );
}
   return 0;


}
