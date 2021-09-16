// grades2labmark
// Sean O'Shea
// 16/5/17

#include <stdlib.h>
#include <stdio.h>

double grades2labmark(char grades[]){
   int count = 0;
   double AA = 1.2;
   int A = 1; 
   double B = 0.8;
   double C = 0.5;
   double grade = 0;
   while (grades[count] != '\0'){
      if (grades[count] == 'C'){
         grade = grade + C;
      } else if (grades[count] == 'B'){
         grade = grade + B;
      } else if (grades[count] == 'A' && grades[count + 1] != '+'){
         grade = grade + A;
      } else if (grades[count] == 'A'){
         grade = grade + AA;
      }  
      count++;
   }
   if (grade > 10){
      grade = 10;
   }
   return grade;   
}
int main (int argc, char *argv[]){
   printf("%.1lf\n", grades2labmark(argv[1]));
   return 0;
}
