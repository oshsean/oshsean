#include <webots/robot.h>
#include <stdio.h>
#include <string.h>
#include <webots/distance_sensor.h>
#include <webots/position_sensor.h>
#include <webots/motor.h>
#include <math.h>

#define TIME_STEP 32
#define FORWARD_ROTATION 8.25
#define MAX_SPEED 6.28
#define MAX_ERROR 0.0001

#define PATH_PLAN_FILE_NAME "../../PathPlan.txt"
#define MAXCHAR 60

int main(int argc, char **argv) {
 
  wb_robot_init();

  //Calculation for required wheel rotation to turn 90 degrees
  double turn_rotation = M_PI*56.6/80;

  printf("Start - Read path plan from %s\n",PATH_PLAN_FILE_NAME);
  //Note path plan file currently being read from local working folder (same folder as controller)
  
  //Open Path Plan File
  FILE *file;
  char str[MAXCHAR];
  char* filename = PATH_PLAN_FILE_NAME;

  // Error case for unopenable file
  file = fopen(filename, "r");
  if (file == NULL){
    printf("Error: Could not read path planning file\n");
  }
  
  //determining length of path file
  fseek(file,0,SEEK_END);
  int size = ftell(file);
  fseek(file,0,SEEK_SET);
  
  // Print path plan file contents and store it in 'str'   
  while(fgets(str,sizeof(str),file)!=NULL){
     printf("%s\n",str);
  }
  printf("Done - Path plan read!\n");
  fclose(file);
  
  //Separate Path Plan string
  //'initPos' contains first 3 characters of path plan file
  char initPos[3];
  for(int i = 0; i<3;i++){
  initPos[i] = str[i];
  }
  
  //'path' contains path instructions from path plan file
  char path[size-3];
  for (int i = 0; i<sizeof(path);i++){
      path[i] = str[i+3];
  }
  
  //Getting initial position
  //Row/Column Variables
  int row = initPos[0]-'0';
  int column = initPos[1]-'0';
  
  //Getting initial heading
  int j;//Headings iterator 
  char headings[4] = {'N','E','S','W'};
  switch (initPos[2]) {
    case 'N':
      j = 0;
      break;
    case 'E':
      j = 1;
      break;
    case 'S':
      j = 2;
      break;
    case 'W':
      j = 3;
      break;
    default:
      j = 2;
      break;
  }
  
  char current_heading = headings[j];
  
  //Initialising motor devices
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  WbDeviceTag left_motor_sensor = wb_robot_get_device("left wheel sensor");
  WbDeviceTag right_motor_sensor = wb_robot_get_device("right wheel sensor"); 
  wb_position_sensor_enable(left_motor_sensor, TIME_STEP);
  wb_position_sensor_enable(right_motor_sensor, TIME_STEP);
   
  // Enable distance sensors 
  WbDeviceTag front_distance_sensor = wb_robot_get_device("DSF");
  WbDeviceTag right_distance_sensor = wb_robot_get_device("DSR");
  WbDeviceTag left_distance_sensor = wb_robot_get_device("DSL");
  wb_distance_sensor_enable(front_distance_sensor, TIME_STEP);
  wb_distance_sensor_enable(right_distance_sensor, TIME_STEP);
  wb_distance_sensor_enable(left_distance_sensor, TIME_STEP);
  
  //Variables denoting existence of surrounding walls
  char F_Wall;
  char R_Wall;
  char L_Wall;
  
  //Initial wall detection for Step 00
  wb_robot_step(TIME_STEP);   
  switch ((int)wb_distance_sensor_get_value(front_distance_sensor)){
    case 600 ... 800:
      F_Wall = 'Y';
      break;
    default:
      F_Wall = 'N';
      break;
  }
  switch ((int)wb_distance_sensor_get_value(right_distance_sensor)){
    case 600 ... 800:
      R_Wall = 'Y';
      break;
    default:
      R_Wall = 'N';
      break;
  }
  switch ((int)wb_distance_sensor_get_value(left_distance_sensor)){
    case 600 ... 800:
      L_Wall = 'Y';
      break;
    default:
      L_Wall = 'N';
      break;
  }  
  printf("Step: 0, Row: %d, Column: %d, Heading: %c, Left Wall: %c, Front Wall: %c, Right Wall: %c\n", row, column,current_heading,L_Wall,F_Wall,R_Wall);
  //******************************************************************************
  //******************************************************************************
  //MAIN LOOP
  
  if(wb_robot_step(TIME_STEP) != -1){
  
    double L_Target;
    double R_Target;
    int delay;
    
    for(int i = 0; i<sizeof(path); i++) {  
      //get current position
      wb_robot_step(TIME_STEP);
      double current_L_Pos = wb_position_sensor_get_value(left_motor_sensor);
      double current_R_Pos = wb_position_sensor_get_value(right_motor_sensor);
      
      //Determine current position commands
      //Determine current time delay required for set_position sync
      //Adjust position if moving forward
      //Adjust heading if rotating
      switch(path[i]) {
        case 'F':
          L_Target = current_L_Pos + FORWARD_ROTATION;
          R_Target = current_R_Pos + FORWARD_ROTATION; 
          delay = 3600;
          //adjust co-ordinates
          switch (current_heading){
            case 'N':
              row--;
              break;
            case 'E':
              column++;
              break;
            case 'S':
              row++;
              break;
            case 'W':
              column--;
              break;
            default:
              break;        
          }
          break;
        case 'R':
          L_Target = current_L_Pos + turn_rotation;
          R_Target = current_R_Pos - turn_rotation;   
          delay = 1000; 
          //adjust heading
          if(j==3){
            j = 0;
          }
          else {
            j++;
          }
          break;
        case 'L':
          L_Target = current_L_Pos - turn_rotation;
          R_Target = current_R_Pos + turn_rotation;
          delay = 1000;
          //adjust heading
          if(j==0){
            j = 3;
          }
          else {
            j--;
          }
          break;
        default:
          L_Target = current_L_Pos;
          R_Target = current_R_Pos;
          delay = 0;
          printf("No valid path commands\n");
          break;   
     }
          
          //Motor actuation
          wb_motor_set_velocity(left_motor, 0.4*MAX_SPEED);
          wb_motor_set_velocity(right_motor, 0.4*MAX_SPEED);
          wb_motor_set_position(left_motor, L_Target);
          wb_motor_set_position(right_motor, R_Target);
          
          //waiting to sync set_position with current position within error tolerance
          do {
            if (wb_robot_step(TIME_STEP) == -1)
              break;
            delay -= TIME_STEP;
            current_L_Pos = wb_position_sensor_get_value(left_motor_sensor);
            current_R_Pos = wb_position_sensor_get_value(right_motor_sensor);
          } while(fabs(L_Target-current_L_Pos) > MAX_ERROR && fabs(R_Target-current_R_Pos) > MAX_ERROR && delay > 0);
   
    
     //Wall Detection
     wb_robot_step(TIME_STEP);
     wb_robot_step(TIME_STEP);
     
     switch ((int)wb_distance_sensor_get_value(front_distance_sensor)){
       case 600 ... 800:
         F_Wall = 'Y';
         break;
       default:
         F_Wall = 'N';
         break;
     }
     switch ((int)wb_distance_sensor_get_value(right_distance_sensor)){
       case 600 ... 800:
         R_Wall = 'Y';
         break;
       default:
         R_Wall = 'N';
         break;
     }
     switch ((int)wb_distance_sensor_get_value(left_distance_sensor)){
       case 600 ... 800:
         L_Wall = 'Y';
         break;
       default:
         L_Wall = 'N';
         break;
     }
     
     //Heading Adjustment
     current_heading = headings[j];
     
     printf("Step: %d, Row: %d, Column: %d, Heading: %c, Left Wall: %c, Front Wall: %c, Right Wall: %c\n", i+1, row, column,current_heading,L_Wall,F_Wall,R_Wall);    
  }
    
     printf("Done - Path plan executed!\n");
  
  }
  
   wb_robot_cleanup();

 return 0;
}