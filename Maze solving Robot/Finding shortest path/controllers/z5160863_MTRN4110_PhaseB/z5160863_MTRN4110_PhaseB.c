/*
 * File:          z5160863_MTRN4110_PhaseB.c
 * Date:
 * Description:
 * Author:
 * Modifications:
 */

/*
 * You may need to add include files like <webots/distance_sensor.h> or
 * <webots/motor.h>, etc.
 */
#include <webots/robot.h>

/*
 * You may want to add macros here.
 */
#define TIME_STEP 64

#include <stdio.h> 

#define MAP_FILE "../../Map.txt"
#define PATH_PLAN_FILE_NAME "../../PathPlanFound.txt"

#define NUM_ROWS 5
#define NUM_COLS 9
/*
 * This is the main program.
 * The arguments of the main function can be specified by the
 * "controllerArgs" field of the Robot node
 */
 
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
  
  int j = 0;
  
  int Vertical_Walls[NUM_ROWS][NUM_COLS+1];
  int Horizontal_Walls[NUM_ROWS+1][NUM_COLS];
  int col = 0;
  int row = 0;
  int ver_rows = 0;
  int ver_cols = 0;
  int hor_rows = 0;
  int hor_cols = 0;
  
  while (j < i){
    if (row % 2 == 0){
      if (path[j] =='\n'){
      hor_rows = hor_rows + 1;
      hor_cols = 0;
      row = row + 1;
      col = 0;
      } else if ((col-2) % 4 == 0){
        if (path[j] == '-'){
          Horizontal_Walls[hor_rows][hor_cols] = 1;
        } else {
          Horizontal_Walls[hor_rows][hor_cols] = 0;
        }
        hor_cols = hor_cols + 1;
      }
    } else {
      if (path[j] =='\n'){
      ver_rows = ver_rows + 1;
      ver_cols = 0;
      row = row + 1;
      col = 0;
      } else if ((col-1) % 4 == 0){
        if (path[j] == '|'){
          Vertical_Walls[ver_rows][ver_cols] = 1;
        } else {
          Vertical_Walls[ver_rows][ver_cols] = 0;
        }
        ver_cols = ver_cols + 1;
      }
    }
    col = col + 1;
    j = j + 1;
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
   
 
  while (wb_robot_step(TIME_STEP) != -1) {
    /*
     * Read the sensors :
     * Enter here functions to read sensor data, like:
     *  double val = wb_distance_sensor_get_value(my_sensor);
     */

    /* Process sensor data here */

    /*
     * Enter here functions to send actuator commands, like:
     * wb_motor_set_position(my_actuator, 10.0);
     */
  };

  /* Enter your cleanup code here */

  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();

  return 0;
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
        if (Horizontal_Walls[row][col] == 1){
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
        if  (Vertical_Walls[row][col] == 1){
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