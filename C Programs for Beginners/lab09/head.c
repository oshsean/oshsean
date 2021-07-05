// head
// 2/5/17
// Sean O'Shea

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int i = 0;
	int j = 0;
	int lines = 10;
	char * filename;
	if (argv[1][0] == '-'){
		filename = argv[3];	
		lines = atoi(argv[2]);
	} else {
		filename = argv[1];	
	}
	FILE * fh = fopen (filename, "r");
	while (i < lines){
		char arr[1000] = {0};
		fgets(arr, 10000, fh);
		while (arr[j] >= '\n'){
			printf("%c", arr[j]);
			j++;
		}
		j = 0;
		i++;
	} 
	fclose(fh);
	return 0;
}
