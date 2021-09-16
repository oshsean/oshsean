#include "ros/ros.h"
 
#include "std_srvs/Empty.h"
#include "gazebo_msgs/SpawnModel.h"
#include "gazebo_msgs/DeleteModel.h"
 
#include <sstream>
#include <fstream>
 
using namespace std;
 
// This is for interfacing with Moveit move group
#include <moveit/move_group_interface/move_group_interface.h>
 
// These are for the various ROS message formats we need
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32.h"
 
#include "std_msgs/Int32MultiArray.h"
 
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
 
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
 
constexpr double pi = 3.1415926535;
constexpr auto deg_to_rad(double deg) -> double {
return deg * pi/180.0;
}

int wait = 0;
double Arr[2];
double temp_array[2] = {0.5, 0.5};
double column = 6;
double distance1 = column*0.065;
int val3 = 1;
void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr& array);
void chatterCallback(const std_msgs::Int32::ConstPtr& msg);
void movehome(int home, std_srvs::Empty srv, ros::ServiceClient onClient, ros::ServiceClient offClient);
auto spawn_box(ros::NodeHandle n, geometry_msgs::Pose pose) -> bool;
void movebish();

static const std::string model_path = "/home/mtrn4230/major-project-armee/ConnEGGt4_ws/src/armee_world/models/Grid/model.sdf";
 
/* The planning group can be found in the ur5e_epick_moveit_config/config/ur5e.srdf */
static const std::string PLANNING_GROUP = "manipulator";
 
int main(int argc, char** argv)
{
    ros::init(argc, argv, "coordArraySubscriber");
 
    ros::NodeHandle n;    
 
    ros::Subscriber sub3 = n.subscribe("coordArray", 100, arrayCallback);
 
    ros::Subscriber sub = n.subscribe("talker", 1000, chatterCallback); 

    
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
 
    //  ##### MoveIt! setup #####
    //1 home
    //0 col
    //2 storage
    movehome(1, srv, onClient, offClient);
    // ##### Spawn box on table #####
    geometry_msgs::Pose box_pose;
    box_pose.position.x = 1.450;
    box_pose.position.y = 0.000;
    box_pose.position.z = 0.978;
 

    tf2::Quaternion myQuaternion;
    myQuaternion.setRPY( 0, 0, 1.570796 );
 
    box_pose.orientation = tf2::toMsg(myQuaternion);
 
    if(!spawn_box(nh,box_pose)) {
        std::cout<<"grid did not load"<<std::endl;
        return 1;
    }
    while (column != 7) {
        movehome(2, srv, onClient, offClient);
        movehome(1, srv, onClient, offClient);
        movehome(0, srv, onClient, offClient);
        movehome(1, srv, onClient, offClient);
    }
    
    
    //movebish();
    ros::waitForShutdown();
    //ros::shutdown();
 
    return 0;
}
/*
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
*/
 
void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr& array)
{
    // x coord array->data[0]
    // y coord array->data[1]
    //ROS_INFO("x: [%d], y: [%d]", array->data[0], array->data[1]);
    double val1 = array->data[0];
    double val2 = array->data[1];
    //int val3 = array->data[2];
    if (val3 == array->data[2] + 1) {
        temp_array[0] = val1/1000;
        temp_array[1] = val2/1000;
        val3++;
    }
	return;
}

void chatterCallback(const std_msgs::Int32::ConstPtr& msg) {
         ROS_INFO("I heard: [%d]", msg->data);
         column = msg->data;
         distance1 = column*0.065;
         
         wait = 1;
    return;  
}



void movebish() {
    

    }


void movehome(int home, std_srvs::Empty srv, ros::ServiceClient onClient, ros::ServiceClient offClient) {
    auto move_group = moveit::planning_interface::MoveGroupInterface(PLANNING_GROUP);
    auto const* joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
    auto my_plan = moveit::planning_interface::MoveGroupInterface::Plan{};
    if (home == 1) {
        auto group_state = "home";
        move_group.setNamedTarget(group_state);
        // Check if plan is possible
        auto success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        
        // Execute motion to home position
        ROS_INFO("Found path to %s, moving robot...", group_state);
        move_group.move();
    } else if (home == 0) {
        
        geometry_msgs::Pose target_poseA = move_group.getCurrentPose().pose;
        target_poseA= move_group.getCurrentPose().pose;
        target_poseA.position.z += 0.2;
        move_group.setPoseTarget(target_poseA);

        auto success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        if(!success) {
            ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
            return;
        }

        ROS_INFO("Found path up 10 cm, moving robot...");
        move_group.move();
        
        auto group_state = "up";
        move_group.setNamedTarget(group_state);
        // Check if plan is possible
        success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        if(!success) {
            ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
            return;
        }
        
        // Execute motion to up position
        ROS_INFO("Found path to %s, moving robot...", group_state);
        move_group.move();


        ROS_INFO("MOVE TO A POSE GOAL...");

        target_poseA = move_group.getCurrentPose().pose;

        /*
        ROS_INFO("Move to an adjusted joint configuration");
        // Set sholder_pan_joint to 90 deg
        move_group.setJointValueTarget("wrist_2_joint", deg_to_rad(-42.84)); //this is the angle that its off
        // Execute motion
        move_group.move();
        */
        target_poseA.position.z -= 0.06;
        target_poseA.position.x -= 0.079;
        target_poseA.position.y -= 0.01;
        target_poseA.position.y += distance1;
        move_group.setPoseTarget(target_poseA);

        if(!success) {
            ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
            return;
        }

        ROS_INFO("Found to column, moving robot...");
        move_group.move();
        ros::Duration(1).sleep();

        // ##### Move to an adjusted joint configuration #####
        

        offClient.call(srv);
        /*

        ROS_INFO("Move to an adjusted joint configuration");
        // Set sholder_pan_joint to 90 deg
        move_group.setJointValueTarget("wrist_2_joint", deg_to_rad(-132.84)); //this is the angle that its off
        // Execute motion
        move_group.move();
        */

        

    } else {
        auto group_state = "zero";
        move_group.setNamedTarget(group_state);
        
        // Check if plan is possible
        auto success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        if(!success) {
            ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
            return ;
        }
    
        // Execute motion to zero position
        ROS_INFO("Found path to %s, moving robot...", group_state);
        move_group.move();

        ROS_INFO("Moving robot towards counter...");

        // Counter Pick up location
        geometry_msgs::Pose target_pose3 =move_group.getCurrentPose().pose;

        cout << temp_array[0] << endl;
        cout << temp_array[1] << endl;

        target_pose3.position.y += temp_array[0];
        target_pose3.position.x += temp_array[1];
        move_group.setPoseTarget(target_pose3);

        success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        if(!success) {
            ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
            return ;
        }
        ROS_INFO("At counter position..");
        move_group.move();

        geometry_msgs::Pose target_pose1 =move_group.getCurrentPose().pose;

        target_pose1 = move_group.getCurrentPose().pose;
        target_pose1.position.z -= 0.058;
        move_group.setPoseTarget(target_pose1);

        success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        if(!success) {
            ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
            return;
        }

        ROS_INFO("Found path down 15 cm, moving robot...");
        move_group.move();

        ROS_INFO("Turning on the gripper...");
        onClient.call(srv);


        target_pose1 = move_group.getCurrentPose().pose;
        target_pose1.position.z += 0.058;
        move_group.setPoseTarget(target_pose1);

        success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        if(!success) {
            ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
            return ;
        }

        ROS_INFO("Found path up 15 cm, moving robot...");
        move_group.move();

        group_state = "zero";
        move_group.setNamedTarget(group_state);
        
        // Check if plan is possible
        success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        if(!success) {
            ROS_WARN("Unable to plan path. Ensure goal pose is valid or adjust tolerance");
            return ;
        }
    
        // Execute motion to zero position
        ROS_INFO("Found path to %s, moving robot...", group_state);
        move_group.move();


    }
}
//     ros::init(argc, argv, "listener");

//     ros::NodeHandle n;

//     //while (wait != 0){
//     ros::Subscriber sub = n.subscribe("talker", 1000, chatterCallback);  

//    // }
//    // wait++;

    
    

        
    
//     ros::spin();
    
//     return 0;
// }

auto const box_name = "Grid";

// auto delete_box(ros::NodeHandle n) -> bool {

//     ros::ServiceClient deleteModel = n.serviceClient<gazebo_msgs::DeleteModel>("gazebo/delete_model");
//     deleteModel.waitForExistence();
//     gazebo_msgs::DeleteModel srv;

//     srv.request.model_name = box_name;

//     deleteModel.call(srv);

//     return srv.response.success;
// }

// // Author: Max Kelly
auto spawn_box(ros::NodeHandle n, geometry_msgs::Pose pose) -> bool {

    ros::ServiceClient spawnModel = n.serviceClient<gazebo_msgs::SpawnModel>("gazebo/spawn_sdf_model");
    spawnModel.waitForExistence();
    gazebo_msgs::SpawnModel srv;

    srv.request.model_name = box_name;

    // load sdf file
    std::ifstream ifs;
    ifs.open(model_path);
    std::stringstream buffer;
    buffer << ifs.rdbuf();

    srv.request.model_xml = buffer.str();

    srv.request.initial_pose = pose;
    srv.request.robot_namespace = "/";
    srv.request.reference_frame = "world";

    spawnModel.call(srv);
    return srv.response.success;
}

