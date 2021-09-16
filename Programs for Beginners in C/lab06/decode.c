//decode
//6/4/17
//By Sean O'Shea

#include <stdlib.h>
#include <stdio.h>

int decode(char n[26], char letter);
int main(int argc, char *argv[]){
	
	char n[26] = {0};
	char letter = 0;
	int i = 0;

	while (i < 26){
		n[i] = argv[1][i]; 
		i = i + 1;
	}
	while (letter != EOF){
		letter = getchar();
		if (letter != EOF){
			putchar(decode(n, letter));
		}	
	}
}
int decode(char n[26], char letter){
	int p = 0;
	if (letter >= 'a' && letter <= 'z'){
		while ( p < 26){
			if (letter == n[p]){
				letter = p + 'a';
				break;
			}
			p++;
		}
	}
	else if (letter >= 'A' && letter <= 'Z'){
		while ( p < 26){
			if (letter == n[p] - 32){
				letter = p + 'A';
				break;
			}
			p++;
		}
	}
	
	return letter;
}
