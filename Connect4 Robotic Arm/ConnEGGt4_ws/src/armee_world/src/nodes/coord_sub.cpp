#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include "ros/ros.h"

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"

int val3 = 1;

void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr& coord_array);

int main(int argc, char **argv)
{

	ros::init(argc, argv, "coordArraySubscriber");

	ros::NodeHandle n;	

	ros::Subscriber sub3 = n.subscribe("coordArray", 100, arrayCallback);

	ros::spin();
    
	return 0;
}

void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr& array)
{
    // x coord array->data[0]
    // y coord array->data[1]
    ROS_INFO("x: [%d], y: [%d], turn: [%d]", array->data[0], array->data[1], array->data[2]);

	double val1 = array->data[0];
    double val2 = array->data[1];
    //int val3 = array->data[2];
    if (val3 == array->data[2] + 1) {
        std::cout<<val1/1000<<std::endl;
        std::cout<<val2/1000<<std::endl;
        val3++;
    }

	return;
}