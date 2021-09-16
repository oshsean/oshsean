# Launch Instructions
## Base World
If robot needed, remove comment tags in launch file lines 16,18,20 and 22.
```
ln -s  ~/major-project-armee/ConnEGGt4_ws/src/armee_world/models/Store ~/.gazebo/models
ln -s  ~/major-project-armee/ConnEGGt4_ws/src/armee_world/models/Grid ~/.gazebo/models

cd major-project-armee/ConnEGGt4_ws
catkin_make
source devel/setup.bash
roslaunch armee_world armeeworld.launch 
```

## Controller
Load in another terminal
```
source devel/setup.bash

rosrun armee_world armee_world_moveit
```


## Counters 

Load in another terminal
```
source devel/setup.bash
```
For just counters:
```
rosrun armee_world spawn_counters
```
For the grid also, move robot to safe starting position, then load:
```
rosrun armee_world spawn_assets
```
