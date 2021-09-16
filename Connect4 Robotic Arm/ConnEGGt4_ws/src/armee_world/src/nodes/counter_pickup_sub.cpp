#include "ros/ros.h"

#include "std_srvs/Empty.h"
#include "gazebo_msgs/SpawnModel.h"
#include "gazebo_msgs/DeleteModel.h"

#include <sstream>
#include <fstream>


// This is for interfacing with Moveit move group
#include <moveit/move_group_interface/move_group_interface.h>

// These are for the various ROS message formats we need
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

#include "std_msgs/Int32MultiArray.h"

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

constexpr double pi = 3.1415926535;


int Arr[90];
void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr& array);

static const std::string model_path = "/home/mtrn4230/lab_demo_repos/lab09_demo/lab09_gazebo/models/box/box.sdf";

/* The planning group can be found in the ur5e_epick_moveit_config/config/ur5e.srdf */
static const std::string PLANNING_GROUP = "manipulator";

int main(int argc, char** argv)
{
    //  ##### ROS setup #####

    ros::init(argc, argv, "lab09_demo");
    auto nh = ros::NodeHandle{};

    ros::ServiceClient onClient = nh.serviceClient<std_srvs::Empty>("/ur5e_epick/epick/on");
    ros::ServiceClient offClient = nh.serviceClient<std_srvs::Empty>("/ur5e_epick/epick/off");
    std_srvs::Empty srv;

    auto spinner = ros::AsyncSpinner(1);
    spinner.start();

    // ##### ensure gripper off and no box spawned #####
    offClient.call(srv);
    delete_box(nh);


    //  ##### MoveIt! setup #####
    auto move_group = moveit::planning_interface::MoveGroupInterface(PLANNING_GROUP);

    auto const* joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);

    auto my_plan = moveit::planning_interface::MoveGroupInterface::Plan{};

    //  ##### State logging #####

    // We can print the name of the reference frame for this robot.
    ROS_INFO("Planning frame: %s", move_group.getPlanningFrame().c_str());

    // We can also print the name of the end-effector link for this group.
    ROS_INFO("End effector link: %s", move_group.getEndEffectorLink().c_str());

    // We can get a list of all the groups in the robot:
    {
        auto groups_str = std::string{};
        for(auto const & group : move_group.getJointModelGroupNames()) {
            groups_str += group + ", ";
        }
        ROS_INFO("Available Planning Groups: %s", groups_str.c_str());
    }


    // ##### Move to a "named" configuration (named in the srdf) #####
    ROS_INFO("Move to a \"named\" configuration (named in the srdf)");

    // Options are currently, "zero", "home" and "up"
    auto group_state = "home";
    move_group.setNamedTarget(group_state);
    // Check if plan is possible
    auto success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    if(!success) {
        ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
        return 1;
    }

    // Execute motion to home position
    ROS_INFO("Found path to %s, moving robot...", group_state);
    move_group.move();

    ROS_INFO("Moving robot towards counter...");
    move_group.move();

    geometry_msgs::Pose start_pose2 =move_group.getCurrentPose().pose;
    std::vector<geometry_msgs::Pose> waypoints;

    geometry_msgs::Pose target_pose3 = start_pose2;

    target_pose3.position.y += temp_array[0];
    target_pose3.position.x += temp_array[1];
    waypoints.push_back(target_pose3);

    moveit_msgs::RobotTrajectory trajectory;
    const double jump_threshold = 0.0;
    const double eef_step = 0.01;
    double fraction = move_group.computeCartesianPath(waypoints, eef_step, jump_threshold, trajectory);

    ROS_INFO("Executing Trajectory");
    move_group.execute(trajectory);

    move_group.setNamedTarget(group_state);
    // Check if plan is possible
    success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    if(!success) {
        ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
        return 1;
    }

    // Execute motion to home position
    ROS_INFO("Found path to %s, moving robot...", group_state);
    move_group.move();

    target_pose1 = move_group.getCurrentPose().pose;
    target_pose1.position.z -= 0.15;
    move_group.setPoseTarget(target_pose1);

    success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    if(!success) {
        ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
        return 1;
    }

    ROS_INFO("Found path down 15 cm, moving robot...");
    move_group.move();

    ROS_INFO("Turning on the gripper...");
    onClient.call(srv);




    target_pose1 = move_group.getCurrentPose().pose;
    target_pose1.position.z += 0.15;
    move_group.setPoseTarget(target_pose1);

    success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    if(!success) {
        ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
        return 1;
    }

    ROS_INFO("Found path up 15 cm, moving robot...");
    move_group.move();

    target_pose1 = move_group.getCurrentPose().pose;
    target_pose1.position.x -= 0.7;
    target_pose1.position.y += 0.15;
    move_group.setPoseTarget(target_pose1);

    success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    if(!success) {
        ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
        return 1;
    }

    ROS_INFO("Found path up 15 cm, moving robot...");
    move_group.move();


    ROS_INFO("Turning off the gripper...");
    offClient.call(srv);

    ros::shutdown();
    return 0;
}

int main(int argc, char **argv)
{
    int temp_array = [90]
	ros::init(argc, argv, "arraySubscriber");

	ros::NodeHandle n;	

	ros::Subscriber sub3 = n.subscribe("array", 100, arrayCallback);

	ros::spinOnce();

	for(j = 1; j < 90; j++)
	{
		printf("%d, ", Arr[j]);
        temp_array[j] = Arr[j];

	}

	printf("\n");
	return 0;
}

void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr& coord_array)
{

	int i = 0;
	// print all the remaining numbers
	for(std::vector<int>::const_iterator it = coord_array->data.begin(); it != coord_array->data.end(); ++it)
	{
		Arr[i] = *it;
		i++;
	}

	return;
}