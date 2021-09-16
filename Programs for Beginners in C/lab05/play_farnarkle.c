// play_farnarkle
// Written 1/4/2017
// by Sean O'Shea



#include <stdio.h>
#include "farnarkle.h"

int main(void) {

    int hidden[N_TILES];

	create_random_tiles(hidden);

	int farnarkle = 0;
	int arkle = 0;
    int i = 0;
	int t = 1;
	int a[N_TILES];
	
	while (t < 100){
		printf ("Enter guess for turn %d: ", t);
		while (i < N_TILES){
			scanf ("%d", &a[i]);		
			i++;
		}
		farnarkle = count_farnarkles(hidden,a);
		arkle = count_arkles(hidden,a);

		if (farnarkle == 4){
			printf ("%d farnarkles %d arkles \nYou win\n", farnarkle, arkle);
			return 0;
		} else {
			printf ("%d farnarkles %d arkles \n", farnarkle, arkle);
		}
		t++;
		i = 0;	
	}
	return 0;
}

