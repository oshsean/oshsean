// prism
// Written 7/3/2017
// by Sean O'Shea


#include <stdio.h>

int main (void) {

int length =0;
int width =0;
int height =0;

 printf ("Please enter prism length:");
 scanf (" %d", &length);
 printf	(" Please enter prism width:");
 scanf (" %d", &width);
 printf	(" Please enter prism height:");
 scanf (" %d", &height);
 printf(" A prism with sides");
 printf(" %d", length );
 printf(" %d", width); 
 printf(" %d", height); 
 printf(" has:\n");
 int volume = length * width * height;
	printf("Volume      = %d\n", volume);
 int area = 2 * ( height * width + width * length + length * height);
 printf ("Area        = %d\n", area);	
 int edge_length= 4 * (length + width + height);
 printf ("Edge length = %d\n", edge_length);
   return 0;

}
