// one
// 2/5/17
// Sean O'Shea

#include <stdio.h>
#include <stdlib.h>
	
#define MAX_BUFF 2048

int main(void){
   int count = 0;
	char data[MAX_BUFF] = {0};
	
   printf ("Enter a string: ");
   fgets(data, MAX_BUFF, stdin);
   while (count < MAX_BUFF && data[count] != '\n'){
      printf ("%c\n", data[count]); 
      count++;
   }
   return 0;
}
