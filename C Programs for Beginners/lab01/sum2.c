//sum of 2 numbers
//written, 1/3/2017
//Author, Sean O'Shea
// COMP1511

#include <stdio.h>

int main(void) {
	int x, y, sum;
	printf ("enter x: ");
	scanf ("%d", &x);
	printf ("enter y: ");
	scanf ("%d", &y);
	sum = x + y;

	printf("%d", x);
        printf(" + ");
        printf("%d", y);
        printf(" = ");
        printf("%d", sum);
        printf("\n");
    return 0;
}
