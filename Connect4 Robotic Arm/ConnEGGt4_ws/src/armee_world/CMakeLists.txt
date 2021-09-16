cmake_minimum_required(VERSION 3.0.2)
project(armee_world)

add_compile_options(-std=c++17)
set(CMAKE_CXX_EXTENSIONS OFF)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  gazebo_msgs
  geometry_msgs
  sensor_msgs
  tf2
  cv_bridge
  image_transport
  moveit_core
  moveit_ros_planning
  moveit_ros_planning_interface
  roslaunch
)

# Perform checks on launch files too
roslaunch_add_file_check(launch)

find_package(OpenCV 3.4 REQUIRED)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need


catkin_package(
  LIBRARIES
  INCLUDE_DIRS
  CATKIN_DEPENDS
      moveit_core
      moveit_ros_planning_interface
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include

  cam_pub_cpp
  spawn_counters
  coord_sub
  controller
  
  ${catkin_INCLUDE_DIRS}
)

include_directories(${OpenCV_INCLUDE_DIRS})



add_executable(cam_pub_cpp src/nodes/cam_pub_cpp.cpp)
target_link_libraries(cam_pub_cpp ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_executable(spawn_counters src/nodes/spawn_counters_OLD.cpp)
target_link_libraries(spawn_counters ${catkin_LIBRARIES})
add_executable(spawn_assets src/nodes/spawn_assets.cpp)
target_link_libraries(spawn_assets ${catkin_LIBRARIES})
add_executable(coord_sub src/nodes/coord_sub.cpp)
target_link_libraries(coord_sub ${catkin_LIBRARIES})


include_directories(${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIRS})

# ${PROJECT_NAME} expands to armee_world (or whatever the project name is on line 2 of this file)
add_executable(${PROJECT_NAME}_moveit src/nodes/controller.cpp)
target_link_libraries(${PROJECT_NAME}_moveit ${catkin_LIBRARIES} ${Boost_LIBRARIES})

