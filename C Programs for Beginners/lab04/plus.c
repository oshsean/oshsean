// Plus
// Written 21/3/2017
// Author Sean O'Shea


#include <stdio.h>

int main(void){

    int size = 0;  

    printf("Enter size: ");
    scanf("%d", &size);
	
    
	 int row = 1;
	 int col = 1;
	
	
    while (row <= size) {
        col = 1;
        
      if (row == (size+1)/2){    
		
        while (col <= size) {
                                   
               printf("*");          
          
            col = col + 1;
        }
        printf("\n");   
        row = row + 1;
       } 
    
      else {
      
         while (col <= size) {
            
            if (col == (size+1)/2){
            
               printf("*");
            }
            
            else {
               printf ("-");
            }   
            
            col = col + 1;
        }
        printf("\n");   
        row = row + 1;
      }
    }
	


return 0;

}
