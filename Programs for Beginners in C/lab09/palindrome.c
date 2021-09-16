// palindrome
// 2/5/17
// Sean O'Shea

#include <stdio.h>
#include <stdlib.h>
	
#define MAX_BUFF 2048

int main(void){
   int count = 0;
   int copys = 0;
   int check = 0;
   int save = 0;
   char copy[MAX_BUFF] = {0};
   char data[MAX_BUFF] = {0};
	
   printf ("Enter a string: ");
   fgets(data, MAX_BUFF, stdin);
   while (count < MAX_BUFF && data[count] != '\0'){
      count++;
   }
   save = count - 2;
   while (copys < count) {
      copy[copys] = data[copys];
      copys++;
   }
   copys = 0;
   while (save >= 0) {
      if (copy[copys] == data[save]){
         check++;
      } 
      copys++;
      save--;  
   }
   if (check == count-1){
      printf ("String is a palindrome\n");
   } else {
      printf ("String is not a palindrome\n");
   }
   return 0;
}
