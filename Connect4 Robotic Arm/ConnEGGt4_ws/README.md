# Package Making Process
In the src folder, make a package with ***catkin_create_package***. The parameters include the package name first, then all ROS dependancies needed. Note that the package name should be in lowercase.
```
$cd src
$catkin_create_pkg package_name std_msgs rospy roscpp geometry_msgs
```
The cpp code is made in the packages src folder. After, configure the **CMakeLists.txt** and the **package.xml** if needed.

# To run the UI 
Go to ConnEGGt4_ws 
```
$catkin make
$source devel/setup.bash
$roslaunch pubsandsubs tl.launch
```
thats ittttt - best way to end this is to go to terminal and do crtl c enter

