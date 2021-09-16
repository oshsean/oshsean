//print world
//Sean O'Shea
//9/5/17

#include "trader_bot.h"
#include <stdio.h>

void print_world(struct bot *b){
   struct location * start = NULL;
   
   while(start != b-> location){
      if (start == NULL){
         start = b->location;
      }
      printf("%s: ", start->name);
      if (start->type == LOCATION_START){
        printf("start\n");
      } else if (start->type == LOCATION_SELLER){
         printf ("will sell %d units of %s for $%d\n", start->quantity, start->commodity->name, start->price);
      } else if (start->type == LOCATION_DUMP){
         printf ("dump\n");
      } else if (start->type == LOCATION_OTHER){
         printf ("other\n");
      } else if (start->type == LOCATION_PETROL_STATION){
         printf ("Petrol station %d units of available fuel for $%d\n", start->quantity, start->price);
      } else if (start->type == LOCATION_BUYER){
         printf ("will buy %d units of %s for $%d\n", start->quantity, start->commodity->name, start->price);
      }
      start = start->next;
   }
}


