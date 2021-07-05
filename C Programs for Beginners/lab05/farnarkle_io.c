// farnarkle
// Written 27/3/2017
// by Sean O'Shea

#include <stdio.h>
#include "farnarkle.h"


int read_tiles(int tiles[N_TILES]){


	int digit = 0;
    int i = 0;
		     
	   
	while (i < N_TILES){
	   if (scanf ("%d", &digit) != 1){
			return 0;
	   }
	   if (digit <= MAX_TILE){
	      
	      tiles[i] = digit;
	      
	      i = i + 1;
	   }   
	   else{
	      return 0;
	      
	   }
	}
	  
   return 1;
}
   
   
void print_tiles(int tiles[N_TILES]){
   
   int i = 0;
   
   while (i < N_TILES){
      
         printf ("%d ", tiles[i]);
         i = i + 1;
   }
   printf ("\n");
}


