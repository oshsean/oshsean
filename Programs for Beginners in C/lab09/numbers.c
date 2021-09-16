// numbers
// 2/5/17
// Sean O'Shea

#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFF 2048

int main(int argc, char *argv[]){

	int min = atoi(argv[1]);
	int max = atoi(argv[2]);
	char * filename = argv[3];
	FILE * fh = fopen (filename, "w");

	while (min <= max){
		fprintf(fh, "%d\n", min);
		min++;	
	}
	fclose(fh);
	return 0;
}
