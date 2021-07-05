//fuel_bot
//Sean O'Shea
//9/5/17

#include "trader_bot.h"
#include <math.h>
#include <stdio.h>

void get_action(struct bot *bot, int *action, int *n){
   int distance = nearest_fuel(bot);
	int back_dist = distance;
	int back_fuel = bot->fuel;
	int fuel = bot->fuel;
	int diff = bot->fuel_tank_capacity - bot->fuel;
	int maximum_move = bot->maximum_move;
   if (bot->location->type != LOCATION_PETROL_STATION){
	  if (distance <= bot->maximum_move){
         *action = ACTION_MOVE;
			if (distance < 0 && bot->fuel >= (back_dist)){
            *n = back_dist;
         } else if (distance > 0 && back_fuel >= (distance)){
            *n = distance;
         } else if (distance > 0){
            *n = fuel;
         } else {
				*n = back_fuel;
			}
      } else {
			*action = ACTION_MOVE;
			*n = maximum_move;
		}
   }
	if (bot->location->type == LOCATION_PETROL_STATION){
 	   *action = ACTION_BUY;
		if (bot->fuel < bot->fuel_tank_capacity){
			*n = diff;
   	} else if (bot->fuel_tank_capacity <= bot->maximum_move){
   	   *n = bot->fuel_tank_capacity;
   	} else {
			*action = ACTION_MOVE;
			*n = maximum_move;
		}
	}
}
