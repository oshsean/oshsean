### Specifications 

ARMEE's newest project ConEGGt 4 is an automated robotic arm playing connect 4 from a users input of where to play each move. To complete this goal the project must follw a strict set of requirements and specifications to be able to play the game correctly. Firstly a set of technical terms and predefined locations specific to this project are outlined as they are required for this system to work. 

The ConnEGGt 4 project requires a robotic arm to be able to physically play connect four from a users input of where to place counters on the grid. For this project a unique set of specifications are required to assure the robot is successfully completing each task involved in playing connect four.

1. Limb Actuation:
The robot's workspace must include the grid and playing counters, for the arm to be able to reach and interact with them for the game to be played. Additionally the TCP must be able to rotate each counter and position it correctly over the playing grid.

2. Camera/Visualisation: 
A camera must be placed to relay a visual to the users of the grid and each move made by the robot. The robotic arm must also be aware of the grid and counter position before starting the game as the camera will not relay this information to the robot.

3. Picking up Counters:
The vacuum gripper must be able to position itself over the stack of counters in their predefined location. Then it must pick up a single counter at a time without knocking over the stack of counters or moving them from their position. 

4. Dropping counters: 
The counter must be raised above the grid and positioned over the column that a user input to be played. The vacuum gripper should drop the counter into the correct grid column for the users turn.

5. User Input:
 The user must input a positive integer from 1 to 7  that must correctly be interpreted as the corresponding playing column on the grid for each move.


 These specifications are required to assure the connect four game is being played correctly and the system is fulfilling all of its tasks for each move in the game. If any of these are functioning incorrectly each move could be wrong and the game could not be played.