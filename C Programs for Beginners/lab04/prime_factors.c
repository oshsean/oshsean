// prime_factors
// Written 21/3/2017
// by Sean O'Shea


#include <stdio.h>

int main(void){
    
    int number, div, input, numeral = 0;
    
    printf("Enter number: ");    
    scanf("%d",&input);
    
	div = 2;

	while (input != 0){
        if (input % div != 0){
            div = div + 1;
			if (input == div){
				printf ("%d is prime\n", input);
				return 0;
			}
		}
		else {
			break;
		}
	}
    printf("The prime factorization of %d is: \n", input);
    
	numeral = input;
    div = 2;
    
	while (numeral != 0){
        if (numeral % div != 0){
            div = div + 1;
		}
        else {
            numeral = numeral / div;
            printf("%d",div);
			break;
		}
	}
	
	number = input / div;
	div = 2;

    while (number != 0){
        if (number % div != 0){
            div = div + 1;
		}
        else {
            number = number / div;
            printf(" * %d",div);
            if (number == 1)
                break;
        }
    }
	printf (" = %d\n", input);

    return 0;
}
