// count_farnarkle
// Written 27/3/2017
// by Sean O'Shea

#include <stdio.h>
#include "farnarkle.h"


int count_farnarkles(int sequence1[N_TILES], int sequence2[N_TILES]){

   int i = 0;
   int farnarkleCount = 0;

   while (i < N_TILES){
      
      if (sequence1[i] == sequence2[i]){
      	farnarkleCount =farnarkleCount + 1;
      }
      i = i + 1;
   }
     
    return farnarkleCount; 
}


