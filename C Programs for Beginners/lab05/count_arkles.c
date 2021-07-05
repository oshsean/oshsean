// count_arkles
// Written 1/4/2017
// by Sean O'Shea

#include <stdio.h>
#include "farnarkle.h"


int count_arkles(int hidden[N_TILES], int guess[N_TILES]){
	
   int hiddenC[N_TILES];
   int ih = 0;
   int ig = 0;
   int arkleCount = 0;

    while (ig < N_TILES){
    	hiddenC[ig] = 0;
		ig = ig + 1;
		}
    ig = 0;

    while (ig < N_TILES){
    	while (ih < N_TILES){
			if  (hidden[ih] == guess[ig] && (hidden[ih] != guess[ih] && hidden[ig] != guess[ig]) && hiddenC[ih] != 1){	
				arkleCount = arkleCount + 1;
				hiddenC[ih] = 1;
				ih = N_TILES;
			}
			ih = ih + 1;
		}
		ig = ig + 1;
		ih = 0;
	}

    return arkleCount; 
}

