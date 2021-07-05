// caesar
// 4/4/17
// by Sean O'Shea
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
   
   int letter = 0;
   int shift = 0;
   int shift1 = 1;
   shift1 = atoi(argv[1]);
   int c = 0;
   c = getchar();
   if (shift1 > 0){
      while (shift1 > 26){
	     shift1 = shift1 - 26;
      }
   }
   if (shift1 < 0){
      while (shift1 < 26){
	     shift1 = shift1 + 26;
      }
      shift1 = shift1 - 26;
   }
   shift = shift1;
   while (c != EOF){
      if (c == '\n'){
         printf ("\n");
      }   
      else if (c >= 'a' && c <= 'z'){
         if (c + shift <= 'z'){
            letter = c + shift;  
         }
         else {
            if (shift > 0){
               letter = 'a' + shift + c - 'z' - 1;
            }
         }
         printf ("%c", letter);
      }
      else if (c >= 'A' && c <= 'Z'){
         if (c + shift <= 'Z'){
            letter = c + shift;  
         }
         else {
            letter = 'A' + shift + c - 'Z' - 1;
         }
         printf ("%c", letter);
      }
      else {
         putchar (c);
      }
      c = getchar();   
   }
}
