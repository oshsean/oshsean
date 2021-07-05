// File:          Mapping_extension.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>

// All the webots classes are defined in the "webots" namespace
using namespace webots;

// This is the main program of your controller.
// It creates an instance of your Robot instance, launches its
// function(s) and destroys it at the end of the execution.
// Note that only one instance of Robot should be created in
// a controller program.
// The arguments of the main function can be specified by the
// "controllerArgs" field of the Robot node
int main(int argc, char **argv) {
  // create the Robot instance.
  Robot *robot = new Robot();

  // get the time step of the current world.
  int timeStep = (int)robot->getBasicTimeStep();

  // You should insert a getDevice-like function in order to get the
  // instance of a device of the robot. Something like:
  //  Motor *motor = robot->getMotor("motorname");
  //  DistanceSensor *ds = robot->getDistanceSensor("dsname");
  //  ds->enable(timeStep);

  // Main loop:
  
  
  int Vertical_Walls_Found[num_rows][num_cols+1];
  int Horizontal_Walls_Found[num_rows+1][num_cols];
  
  
  for (int a = 0; a < num_rows; a++) {
    for (int b = 0; b < num_cols; b++) {
      Vertical_Walls_Found[a][b] = 0;
      Horizontal_Walls_Found[a][b] = 0;
    }
  }  
  
void Mapping_Walls(int current_heading, int num_rows, int num_cols, int row, int col){
  
  
  if (current_heading == 'N'){
  //North
    if(L_Wall == 'Y'){
      Vertical_Walls_Found[row][col-1] = 1;
    }
    if(R_Wall == 'Y'){
      Vertical_Walls_Found[row][col+1] = 1;
    }
    if(F_Wall == 'Y'){
      Horizontal_Walls_Found[row-1][col] = 1
    }
  } else if (current_heading == 'E'){
  //East
    if(L_Wall == 'Y'){
      Horizontal_Walls_Found[row][col-1] = 1;
    }
    if(R_Wall == 'Y'){
      Horizontal_Walls_Found[row][col+1] = 1;
    }
    if(F_Wall == 'Y'){
      Vertical_Walls_Found[row+1][col] = 1
    }
  } else if (current_heading == 'S'){
  //South
    if(L_Wall == 'Y'){
      Vertical_Walls_Found[row][col+1] = 1;
    }
    if(R_Wall == 'Y'){
      Vertical_Walls_Found[row][col-1] = 1;
    }
    if(F_Wall == 'Y'){
      Horizontal_Walls_Found[row+1][col] = 1
    }
  } else if (current_heading == 'W'){
  //West
    if(L_Wall == 'Y'){
      Horizontal_Walls_Found[row+1][col] = 1;
    }
    if(R_Wall == 'Y'){
      Horizontal_Walls_Found[row-1][col] = 1;
    }
    if(F_Wall == 'Y'){
      Vertical_Walls_Found[row][col-1] = 1
    }
  }
}  
  
  // - perform simulation steps until Webots is stopping the controller
  while (robot->step(timeStep) != -1) {
    // Read the sensors:
    // Enter here functions to read sensor data, like:
    //  double val = ds->getValue();

    // Process sensor data here.

    // Enter here functions to send actuator commands, like:
    //  motor->setPosition(10.0);
  };

  // Enter here exit cleanup code.

  delete robot;
  return 0;
}
