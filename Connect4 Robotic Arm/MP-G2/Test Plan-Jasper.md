### Test Plan
Documents the procedures used to evaluate performance of the project solution against the project specifications. At the end of the term the team will document the test results and these should show to what degree the project specifications have been met.
**Technical Terms**
*    The Red and Yellow pieces of the connect four game will be referred to as 'Counters'
*    The Blue 6x7 Connect Four board will be referred to as 'The Grid'
*    'The Grid' is made up of 6 rows and 7 columns
*    The end effector of the robot for this project is a vaccuum gripper
*    The predefined spaces next to the robot where 'Counters' will be stored, will be known as 'Storage areas'. 

**Predefined Locations:**
 *  Two 'Storage areas' will be located on either side of the robot base, one for red counters and one for yellow counters.
 *  The Storage areas will be pre-located to maximise ease of access by the robot
 *   Counters in storage areas will be stacked on top of eachother in a similar way to pancakes for ease of palletization.
 *   The Camera will be positioned such that it may record the entire board and all moves made by the robot.
 *   The Camera will relay its visuals to the users.
 *  The Drop location is a predefined location, defined relative to the Grid, above each of the columns from which the counters will be dropped. The Drop location is defined such that a counter released from the TCP should fall into the column of the Grid that the Drop location is defined relative to.
 *  The Pick up location is a predefined location, defined relative to the Storage area, above each of the counter stacks from which the counters will be picked up.
 
**Robot Functionality Breakdown:**
Input: User Command (Column from 0-6).
1. Relate Command to a column to a predefined position
2. Determine if it is red or yellow's turn (yellow goes first)
3. Move to yellow storage area (if even turn) or to red storage area (if odd turn)
4. Move down and grip counter using the end effector (Vaccuum gripper)
5. Move the end effector to the position of the user defined column position
6. Drop the counter into the user defined column
7. Return to home position
Output: Counter placed in Column (defined by User input of 0-6).

**Testing Types**
1. Limb Acctuation Accuracy Testing:
* 1.1a The Testing Plan* includes attatching a 'counter' to the TCP of the gripper and monitoring the robot's movement from the Storage area to the Drop location (see the definition for the Drop location). The TCP's final orientation and position relative to the top of the column and the Drop location is recorded, specifically whether the TCP is correctly rotated so the Counter is positioned directly above the desired column. This action is repeated 50 times.
* 1.1b The Acceptance criteria* for this test plan is that the average orientaiton and position of the counter differs from the Drop location and orientation by no more than 3mm. If the TCP position conforms to the Drop location parameters but is visibly not aligned with the top of the desired column of the grid, this test is considered a failure and the Drop position must be redefined.

2. Camera/Visual testing:
* 2.1a The Testing Plan* includes timing the image refresh over ten minutes and taking the average-minimum refresh time to ensure the camera is relaying information to the players at a rate that allows them to observe the board and the moves being made. 
* 2.1b The Acceptance criteria* for this test plan is an average image refresh rate that is < 15 Hz. If the average image refresh time is greater than 15 Hz, this is considered a failure and a new camera will have to be sourced.
* 2.2a The Testing plan* includes prompting the user for a move via the UI and monitoring the software values including the turn counter and desired column variables.
* 2.2b The Acceptance criteria* for this test plan is that the software passes all UNIT tests designed by the software engineers, the specifics of these UNIT tests are omited from this report to ensure industrial espionage is minimised. If the UI fails to complete any of the UNIT tests it will be considered a failure and more debugging must be undergone.

3. Vaccuum Grip Testing - Counter Drop Testing:
* 3.1a The Testing plan* involves moving the robot to each of the Drop locations, with a Counter attatched to the gripper, and dropping the counter 10 times into each column. The number if counters that failed to reach their desired destination will be tallied, this includes counters that land in their desired position and then bounce off.
* 3.2b The Acceptance criteria* for this test plan is 0 Counters failing to reach their desired position. If any counters fail to reach their destination, the Drop location dimensions will be revised. If counters travel to their desired position before bouncing into a different position that will also be considered a failure, however should multiple Counters experience this behaviour the dimensions of the Grid itself will be revised.
        
4. Vaccuum Grip Testing - Counter Pick Up Testing: 
* 4.1a The Testing plan* involves moving the robot to the pick up position, picking up a counter and holding it for 15 seconds. Remove the counter and repeat 50 times, tally all failed pick up actions including successful pick ups followed by immediate drops.
* 4.2b The Acceptance criteria* for this test plan is 0 Counters dropped and the position of the counter on the gripper never exceeding a 3mm radius around the TCP. Should any number of counters be dropped, the vaccuum gripper will have to be reviewed and possibly replaced. Similarly if more than 5 counters are collected on the gripper outside the 3mm radius, that will be considered a failure and the palletising controller of the motion will have to be revised. If the gripper collides with the counter stacks in a destructive way that causes it to shift, topple or lose counters, that will also be considered a failure, in this case the robot path and pick up location will have to be revised.

5. Holistic Motion Testing:
* 5.1a The Testing plan* involves the combining of all components of the test plan, to input a column value, pick up a counter, move to the drop location and drop a counter into the user specified column. This test simulates the full range of motion and actions the final product will undertake, with the overall goal of having the robot play connect 4.
* 5.1b The Acceptance criteria* for this test plan is for the correct coloured counter to be picked up without issue, moved without violating safety constraints to the user specified drop location and dropped into the correct position. Any deviation visible or otherwise, from the full, correct operation of the robot will be considered a failure and the testing staff will categorise the source of the error and revise the appropriate sub-system. 
