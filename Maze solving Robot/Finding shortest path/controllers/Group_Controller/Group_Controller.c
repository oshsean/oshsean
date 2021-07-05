//Group Controller


#define TIME_STEP 32
#define FORWARD_ROTATION 8.25
#define MAX_SPEED 6.28
#define MAX_ERROR 0.0001

#include <stdio.h> 
#include <webots/robot.h>
#include <string.h>
#include <webots/distance_sensor.h>
#include <webots/position_sensor.h>
#include <webots/motor.h>
#include <math.h>

#define MAP_FILE "../../Map.txt"
#define PATH_PLAN_FILE_NAME "../../PathPlanFound.txt"

#define MAXCHAR 60


#define NUM_ROWS 5
#define NUM_COLS 9
/*
 * This is the main program.
 * The arguments of the main function can be specified by the
 * "controllerArgs" field of the Robot node
 */
void Mapping_Walls(
int current_heading, 
int num_rows, 
int num_cols, 
int row, 
int col,
int Horizontal_Walls_Found[NUM_ROWS+1][NUM_COLS],
int Vertical_Walls_Found[NUM_ROWS][NUM_COLS+1],
char L_Wall, 
char R_Wall, 
char F_Wall
);
 
void print_path(
  int initial,
  int start_row,
  int start_col,
  int num_rows, 
  int num_cols, 
  int Horizontal_Walls[NUM_ROWS+1][NUM_COLS],
  int Vertical_Walls[NUM_ROWS][NUM_COLS+1], 
  int solution[NUM_ROWS][NUM_COLS]
);

void write_path_to_file(
  int initial,
  int start_row,
  int start_col,
  int num_rows, 
  int num_cols, 
  int Horizontal_Walls[NUM_ROWS+1][NUM_COLS],
  int Vertical_Walls[NUM_ROWS][NUM_COLS+1], 
  int least_turns_path[NUM_ROWS][NUM_COLS]
);
 
int main(int argc, char **argv) {
  /* necessary to initialize webots stuff */
  wb_robot_init();
  
  //TASK 1
  
  printf("--- Task 1 ---\n");
  
  int num_rows = 0;
  int num_cols = 0;
  int start_row = 0;
  int start_col = 0;
  int compass = 0;
  int initial = 0;
  
  FILE *fp;
  fp = fopen(MAP_FILE, "r");
  char path[5000];
  int i = 0;
  while (1) {
    path[i] = fgetc(fp);
    if(path[i]==EOF){
      num_cols = num_cols - 2;
      num_rows = num_rows/2;
      num_cols = num_cols/4;
      printf("\n");
      break;
            
    }else{
      printf("%c", path[i]);
      if (path[i] =='\n'){
        num_rows = num_rows + 1;
        num_cols = 0;
      } else if (path[i] == 'v'){
        start_row = (num_rows/2) + 1;
        start_col = ((num_cols - 2)/4) + 1;
        initial = 1;
        compass = 1;
      } else if (path[i] == '^'){
        start_row = (num_rows/2) + 1;
        start_col = ((num_cols - 2)/4) + 1;
        initial = 3;
        compass = 3;
      } else if (path[i] == '<'){
        start_row = (num_rows/2) + 1;
        start_col = ((num_cols - 2)/4) + 1;
        initial = 2;
        compass = 2;
      } else if (path[i] == '>'){
        start_row = (num_rows/2) + 1;
        start_col = ((num_cols - 2)/4) + 1;
        initial = 4;
        compass = 4;
      }
      num_cols = num_cols + 1;
      i = i + 1;
    }
 }
  fclose( fp );
  
  
  //TASK 2
  
  
  printf("--- Task 2 ---\n");
  
  int d = 0;
  
  int Vertical_Walls[NUM_ROWS][NUM_COLS+1];
  int Horizontal_Walls[NUM_ROWS+1][NUM_COLS];
  int col = 0;
  int row = 0;
  int ver_rows = 0;
  int ver_cols = 0;
  int hor_rows = 0;
  int hor_cols = 0;
  
  while (d < i){
    if (row % 2 == 0){
      if (path[d] =='\n'){
      hor_rows = hor_rows + 1;
      hor_cols = 0;
      row = row + 1;
      col = 0;
      } else if ((col-2) % 4 == 0){
        if (path[d] == '-'){
          Horizontal_Walls[hor_rows][hor_cols] = 1;
        } else {
          Horizontal_Walls[hor_rows][hor_cols] = 0;
        }
        hor_cols = hor_cols + 1;
      }
    } else {
      if (path[d] =='\n'){
      ver_rows = ver_rows + 1;
      ver_cols = 0;
      row = row + 1;
      col = 0;
      } else if ((col-1) % 4 == 0){
        if (path[d] == '|'){
          Vertical_Walls[ver_rows][ver_cols] = 1;
        } else {
          Vertical_Walls[ver_rows][ver_cols] = 0;
        }
        ver_cols = ver_cols + 1;
      }
    }
    col = col + 1;
    d = d + 1;
  }
    
  int max = num_rows*num_cols;
  int paths[NUM_ROWS][NUM_COLS];
  
  //set all to max distance
  for (int a = 0; a < num_rows; a++) {
    for (int b = 0; b < num_cols; b++) {
      paths[a][b] = max;
    }
  }
  
  //flood fill to find shortest path
  
  paths[(num_rows-1)/2][(num_cols-1)/2] = 0;  
  int change = 1;
  int explore = 0;
  int c = 0;
  while (change != 0){
    change = 0;
    for (int row = 0; row < num_rows; row++) {
      for (int col = 0; col < num_cols; col++) {
        if (paths[row][col] == explore){
          for (c = 1; c <= 4; c++){
            switch(c){
              case(1): //S
                if (Horizontal_Walls[row+1][col] == 0){
                  if (paths[row+1][col] == max){
                    paths[row+1][col] = paths[row][col] + 1;
                    change = 1;
                  }
                }
              case(2): //W
                if (Vertical_Walls[row][col] == 0){
                  if (paths[row][col-1] == max){
                    paths[row][col-1] = paths[row][col] + 1;
                    change = 1;
                  }
                }
              case(3): //N
                if (Horizontal_Walls[row][col] == 0){
                  if (paths[row-1][col] == max){
                    paths[row-1][col] = paths[row][col] + 1;
                    change = 1;
                  }
                }
              case(4): //E
                if (Vertical_Walls[row][col+1] == 0){
                  if (paths[row][col+1] == max){
                    paths[row][col+1] = paths[row][col] + 1;
                    change = 1;
                  }
                }
            }
          }
        }
      }
    }
    explore = explore + 1;
  }
 
 
  int solution[NUM_ROWS][NUM_COLS];
  
  for (int a = 0; a < num_rows; a++) {
    for (int b = 0; b < num_cols; b++) {
      solution[a][b] = 0;
    }
  }
  
  int other_paths[NUM_ROWS][NUM_COLS];
  
  for (int a = 0; a < num_rows; a++) {
    for (int b = 0; b < num_cols; b++) {
      other_paths[a][b] = 0;
    }
  }
  
  int least_turns_path[NUM_ROWS][NUM_COLS];
  int current_min_turns = max*2;
  int turns = 0;
  int num_shortest_paths = 0;
  int end = 0;
  int start = 0;
  int compass_prev = initial;
  
  row = start_row-1;
  col = start_col-1;
  change = 1;
  while (end != 1){
    while (change != 0){
      change = 0;
        for (c = 1; c <= 4; c++){
          if (c == 1){ //S
            if (Horizontal_Walls[row+1][col] == 0){
              if (paths[row+1][col] == paths[row][col] - 1){
                other_paths[row+1][col] = 1;
                if (change == 1){
                  other_paths[row][col] = 2;
                }
                change = 1;
                compass = 1;
              }
            }
          }
          if (c == 2){ //W
            if (Vertical_Walls[row][col] == 0){
              if (paths[row][col-1] == paths[row][col] - 1){
                other_paths[row][col-1] = 1;
                if (change == 1){
                  other_paths[row][col] = 2;
                }
                change = 1;
                compass= 2;
              }
            }
          }
          if (c == 3){ //N
            if (Horizontal_Walls[row][col] == 0){
              if (paths[row-1][col] == paths[row][col] - 1){
                other_paths[row-1][col] = 1;
                if (change == 1){
                  other_paths[row][col] = 2;
                }
                change = 1;
                compass = 3;
              }
            }
          }
          if (c == 4){ //E
            if (Vertical_Walls[row][col+1] == 0){
              if (paths[row][col+1] == paths[row][col] - 1){
                other_paths[row][col+1] = 1;
                if (change == 1){
                  other_paths[row][col] = 2;
                }
                change = 1;
                compass = 4;
              }
            }
          } 
          if (other_paths[row][col] != 2){
            other_paths[row][col] = 0;
          }
          solution[row][col] = paths[row][col];
        }
        if (compass_prev != compass){
          turns = turns + 1;
          if (compass_prev == compass - 2 || compass == compass_prev - 2){
            turns = turns + 1;
          }
        }
        compass_prev = compass;
        if (change == 1){
          if (compass == 1){
            row = row + 1;
          } else if (compass == 2){
            col = col - 1;
          } else if (compass == 3){
            row = row - 1;
          } else if (compass == 4){
            col = col + 1;
          }
        }
    }
    num_shortest_paths = num_shortest_paths + 1;
   
      
    if (turns <= current_min_turns){
      current_min_turns = turns;
      for (int a = 0; a < num_rows; a++) {
        for (int b = 0; b < num_cols; b++) {
          least_turns_path[a][b] = solution[a][b];
        }
      }
    }  
    
    printf("--- Path %d ---\n", num_shortest_paths); 
    print_path(initial, start_row, start_col, num_rows, num_cols, Horizontal_Walls, Vertical_Walls, solution); 

      
    while(change != 1){
      change = 1;
      
      for (c = 1; c <= 4; c++){
        if (c == 1){ //S
          if (Horizontal_Walls[row+1][col] == 0){
            if (solution[row+1][col] == solution[row][col] + 1){
              solution[row][col] = 0;
              compass = 3;
              change = 0;
            }
          }
        }
        if (c == 2){ //W
          if (Vertical_Walls[row][col] == 0){
            if (solution[row][col-1] == solution[row][col] + 1){
              solution[row][col] = 0;
              compass = 4;
              change = 0;
            }
          }
        }
        if (c == 3){ //N
          if (Horizontal_Walls[row][col] == 0){
            if (solution[row-1][col] == solution[row][col] + 1){
              solution[row][col] = 0;
              compass = 1;
              change = 0;
            }
          }
        }
        if (c == 4){ //E
          if (Vertical_Walls[row][col+1] == 0){
            if (solution[row][col+1] == solution[row][col] + 1){
              solution[row][col] = 0;
              compass = 2;
              change = 0;
            }
          }
        }
      }
      if (compass_prev != compass){
        turns = turns - 1;
      }
      compass_prev = compass;
      if (change == 0){
        if (compass == 1){
          row = row - 1;
        } else if (compass == 2){
          col = col + 1;
        } else if (compass == 3){
          row = row + 1;
        } else if (compass == 4){
          col = col - 1;
        }
      }
      if (other_paths[row][col] == 2){
        if (row == start_row-1 && col == start_col-1){
          start = 1;
          turns = 0;
        }
        other_paths[row][col] = 0;
        if (other_paths[row+1][col] == 1 && Horizontal_Walls[row+1][col] == 0){
          if  (solution[row-1][col] != solution[row][col]+1){
            turns = turns + 1;
          } else {
            turns = turns -1;
          }
          other_paths[row+1][col] = 0;
          change = 1;
          row = row + 1;
          compass_prev = 1;
        } else if (other_paths[row][col-1] == 1 && Vertical_Walls[row][col] == 0){
          if  (solution[row][col+1] != solution[row][col]+1){
            turns = turns + 1;
          } else {
            turns = turns -1;
          }
          other_paths[row-1][col] = 0;
          change = 1;
          col = col - 1;
          compass_prev = 2;
        } else if (other_paths[row-1][col] == 1 && Horizontal_Walls[row][col] == 0){
          if  (solution[row+1][col] != solution[row][col]+1){
            turns = turns + 1;
          } else {
            turns = turns -1;
          }
          other_paths[row-1][col] = 0;
          change = 1;
          row = row - 1;
          compass_prev = 3;
        } else if (other_paths[row][col+1] == 1 && Vertical_Walls[row][col+1] == 0){
          if  (solution[row][col-1] != solution[row][col]+1){
            turns = turns + 1;
          } else {
            turns = turns -1;
          }
          other_paths[row-1][col] = 0;
          change = 1;
          col = col + 1;
          compass_prev = 4;
        }
        if (start == 1 ){
          start = 0;
          if (compass_prev == initial){
            turns = turns - 1;
          } else if (compass_prev == initial - 2 || compass_prev - 2 == initial){
            turns = turns + 1;
          }
        }
      }
      if (change == 0 && row == start_row-1 && col == start_col-1){
        change = 1;
        end = 1;
      }
    }
  }

  //TASK 3

  printf("--- Task 3 ---\n");
  print_path(initial, start_row, start_col, num_rows, num_cols, Horizontal_Walls, Vertical_Walls, least_turns_path); 
  printf("Steps: %d\n", current_min_turns+least_turns_path[start_row-1][start_col-1]);
  printf("Path: ");
  write_path_to_file(initial, start_row, start_col, num_rows,  num_cols,  Horizontal_Walls, Vertical_Walls, least_turns_path);
  
  
  //TASK 4
  
  printf("--- Task 4 ---\nFile: ./PathPlanFound.txt\n");
  printf("Path: ");
  
  write_path_to_file(initial, start_row, start_col, num_rows,  num_cols,  Horizontal_Walls, Vertical_Walls, least_turns_path);
   
 
//////

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
  char Path[size-3];
  for (int i = 0; i<sizeof(Path);i++){
      path[i] = str[i+3];
  }
  
  //Getting initial position
  //Row/Column Variables
  int Row = initPos[0]-'0';
  int Column = initPos[1]-'0';
  
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
  
  int Vertical_Walls_Found[num_rows][num_cols+1];
  int Horizontal_Walls_Found[num_rows+1][num_cols];
  
  
  for (int a = 0; a < num_rows; a++) {
    for (int b = 0; b < num_cols; b++) {
      Vertical_Walls_Found[a][b] = 0;
      Horizontal_Walls_Found[a][b] = 0;
    }
  }  
  
  printf("Step: 0, Row: %d, Column: %d, Heading: %c, Left Wall: %c, Front Wall: %c, Right Wall: %c\n", Row, Column,current_heading,L_Wall,F_Wall,R_Wall);
  //******************************************************************************
  //******************************************************************************
  //MAIN LOOP
  
  if(wb_robot_step(TIME_STEP) != -1){
  
  
    double L_Target;
    double R_Target;
    int delay;
    
    for(int i = 0; i<sizeof(Path); i++) {  
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
              Row--;
              break;
            case 'E':
              Column++;
              break;
            case 'S':
              Row++;
              break;
            case 'W':
              Column--;
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
      
     Mapping_Walls(current_heading, num_rows, num_cols, Row, Column, Horizontal_Walls_Found, Vertical_Walls_Found, L_Wall, R_Wall, F_Wall);
     printf("Step: %d, Row: %d, Column: %d, Heading: %c, Left Wall: %c, Front Wall: %c, Right Wall: %c\n", i+1, Row, Column,current_heading,L_Wall,F_Wall,R_Wall);    
  }
    
     printf("Done - Path plan executed!\n");
     
     
     
     printf("Path detected by sensors: \n\n");
     print_path(initial, start_row, start_col, num_rows, num_cols, Horizontal_Walls_Found, Vertical_Walls_Found, least_turns_path); 
  }
  
   wb_robot_cleanup();

 return 0;
}





//////

void Mapping_Walls(int current_heading, int num_rows, int num_cols, int row, int col,int Horizontal_Walls_Found[NUM_ROWS+1][NUM_COLS], int Vertical_Walls_Found[NUM_ROWS][NUM_COLS+1], char L_Wall, char R_Wall, char F_Wall){
  
  
  if (current_heading == 'N'){
  //North
    if(L_Wall == 'Y'){
      Vertical_Walls_Found[row][col] = 1;
    }
    if(R_Wall == 'Y'){
      Vertical_Walls_Found[row][col+1] = 1;
    }
    if(F_Wall == 'Y'){
      Horizontal_Walls_Found[row][col] = 1;
    }
  } else if (current_heading == 'E'){
  //East
    if(L_Wall == 'Y'){
      Horizontal_Walls_Found[row][col] = 1;
    }
    if(R_Wall == 'Y'){
      Horizontal_Walls_Found[row+1][col] = 1;
    }
    if(F_Wall == 'Y'){
      Vertical_Walls_Found[row][col+1] = 1;
    }
  } else if (current_heading == 'S'){
  //South
    if(L_Wall == 'Y'){
      Vertical_Walls_Found[row][col+1] = 1;
    }
    if(R_Wall == 'Y'){
      Vertical_Walls_Found[row][col] = 1;
    }
    if(F_Wall == 'Y'){
      Horizontal_Walls_Found[row+1][col] = 1;
    }
  } else if (current_heading == 'W'){
  //West
    if(L_Wall == 'Y'){
      Horizontal_Walls_Found[row+1][col] = 1;
    }
    if(R_Wall == 'Y'){
      Horizontal_Walls_Found[row][col] = 1;
    }
    if(F_Wall == 'Y'){
      Vertical_Walls_Found[row][col] = 1;
    }
  }
}  


void print_path(
  int initial,
  int start_row,
  int start_col,
  int num_rows, 
  int num_cols, 
  int Horizontal_Walls[NUM_ROWS+1][NUM_COLS],
  int Vertical_Walls[NUM_ROWS][NUM_COLS+1], 
  int solution[NUM_ROWS][NUM_COLS]
) {
  int row = 0;
  int col = 0;
  int check = 0;
  while(check <= num_rows*2){
  col = 0;
    if (check%2 == 0){
      while (col <= num_cols-1){
        if (Horizontal_Walls[row][col] == 1 || row == 0 || row == num_rows){
          printf(" ---");
        } else {
          printf("    ");
        }
        col = col + 1;
      }
      printf(" \n");
      check = check + 1;
    }
    else {
      while (col <= num_cols-1){
        if  (Vertical_Walls[row][col] == 1 || col == 0 || col == num_cols){
          printf("|");
        } else {
          printf(" ");
        } 
        if (row == start_row-1 && col == start_col-1){
          if (initial == 1){
            printf(" v ");
          } else if (initial == 2){
            printf(" < ");
          } else if (initial == 3){
            printf(" ^ ");
          } else if (initial == 4){
            printf(" > ");
          }   
        } else if (solution[row][col] >= 10){
          printf(" %d", solution[row][col]);
        } else if(solution[row][col] > 0){
          printf(" %d ", solution[row][col]);
        } else if (row == num_rows/2 && col == num_cols/2){
          printf(" 0 ");
        } else {
          printf("   ");
        }
        col = col + 1;
        
      }
      row = row + 1;
      check = check + 1;
      printf("| \n"); // assume closed maze
    }
  }
}

void write_path_to_file(
  int initial,
  int start_row,
  int start_col,
  int num_rows, 
  int num_cols, 
  int Horizontal_Walls[NUM_ROWS+1][NUM_COLS],
  int Vertical_Walls[NUM_ROWS][NUM_COLS+1], 
  int least_turns_path[NUM_ROWS][NUM_COLS]
) {
   
   
    FILE *fp;
    fp = fopen(PATH_PLAN_FILE_NAME, "w+");
    
    
    
    fprintf(fp,"%d%d",start_row-1, start_col-1);
    printf("%d%d",start_row-1, start_col-1);
    if (initial == 1){
      fprintf(fp,"S");
      printf("S");
    } else if (initial == 2){
      fprintf(fp,"W");
      printf("W");
    } else if (initial == 3){
      fprintf(fp,"N");
      printf("N");
    } else if (initial == 4){
      fprintf(fp,"E");
      printf("E");
    } 
    
    int row = start_row-1;
    int col = start_col-1;
    int compass = initial;
    
    while (least_turns_path[row][col] != 0){
      for (int c = 1; c <= 4; c++){
        if (c == 1){ //S
          if (Horizontal_Walls[row+1][col] == 0){
            if (least_turns_path[row+1][col] == least_turns_path[row][col] - 1){
              if (compass == 1){
                fprintf(fp,"F");
                printf("F");
              } else if (compass == 2){
                fprintf(fp,"LF");
                printf("LF");
              } else if (compass == 3){
                fprintf(fp,"LLF");
                printf("LLF");
              } else if (compass == 4){
                fprintf(fp,"RF");
                printf("RF");
              } 
              compass = 1;
              row = row + 1;
            }
          }
        }
        if (c == 2){ //W
          if (Vertical_Walls[row][col] == 0){
            if (least_turns_path[row][col-1] == least_turns_path[row][col] - 1){
              if (compass == 1){
                fprintf(fp,"RF");
                printf("RF");
              } else if (compass == 2){
                fprintf(fp,"F");
                printf("F");
              } else if (compass == 3){
                fprintf(fp,"LF");
                printf("LF");
              } else if (compass == 4){
                fprintf(fp,"LLF");
                printf("LLF");
              } 
              compass = 2;
              col = col - 1;
            }
          }
        }
        if (c == 3){ //N
          if (Horizontal_Walls[row][col] == 0){
            if (least_turns_path[row-1][col] == least_turns_path[row][col] - 1){
              if (compass == 1){
                fprintf(fp,"LLF");
                printf("LLF");
              } else if (compass == 2){
                fprintf(fp,"RF");
                printf("RF");
              } else if (compass == 3){
                fprintf(fp,"F");
                printf("F");
              } else if (compass == 4){
                fprintf(fp,"LF");
                printf("LF");
              } 
              compass = 3;
              row = row - 1;
            }
          }
        }
        if (c == 4){ //E
          if (Vertical_Walls[row][col+1] == 0){
            if (least_turns_path[row][col+1] == least_turns_path[row][col] - 1){
              if (compass == 1){
                fprintf(fp,"LF");
                printf("LF");
              } else if (compass == 2){
                fprintf(fp,"LLF");
                printf("LLF");
              } else if (compass == 3){
                fprintf(fp,"RF");
                printf("RF");
              } else if (compass == 4){
                fprintf(fp,"F");
                printf("F");
              } 
              compass = 4;
              col = col + 1;
            }
          }
        }
      } 
    }
  printf("\n");
  fclose( fp );
}