// one_loop_square
// Written 14/3/2017
// by Sean O'Shea
 
#include <stdio.h>

int main(void) {
    int number;
    int row, column;

    printf("Square size: ");
    scanf("%d", &number);

    row = 1;
    while (row <= number) {
        column = 1;
        while (column <= number) {
            printf("*");
            column = column + 1;
        }
        printf("\n");
        row = row + 1;
    }

    return 0;
}
