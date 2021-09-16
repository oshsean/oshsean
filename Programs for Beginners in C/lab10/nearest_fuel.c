//nearest_fuel
//Sean O'Shea
//9/5/17

#include "trader_bot.h"
#include <math.h>
#include <stdio.h>


int nearest_fuel(struct bot *b){
   struct location *curr = NULL;
   int distance_forward = 0;
   int distance_backward = 0;
   if (b->location->type == LOCATION_PETROL_STATION){
      return 0;
   }
   while (curr != b->location){
      if(curr == NULL){
         curr = b->location;  
      }
      if (curr->type == LOCATION_PETROL_STATION){
         break;
      }
      curr=curr->next;
      distance_forward++;
   }
   curr = NULL;
   while (curr != b->location){
      if(curr == NULL){
         curr = b->location;  
      }
      if (curr->type == LOCATION_PETROL_STATION){
         break;
      }
      curr=curr->previous;
      distance_backward--;
   }
   if (distance_forward <= (-1)*(distance_backward)){
      return distance_forward;
   }
   else {
      return distance_backward;
   }
}
