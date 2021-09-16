#include "ros/ros.h"

#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"


#include "std_msgs/String.h"
#include <std_msgs/Float64.h>

#include "gazebo_msgs/SpawnModel.h" 
#include "geometry_msgs/Pose.h" 
#include "geometry_msgs/Point.h" 
#include "geometry_msgs/Quaternion.h" 

#include <sstream>
#include <fstream>
#include <math.h>

#include <chrono>
#include <thread>
int count[3]={0,0,0}; //Global counter {COUNTER, RED COUNTER, YELLOW COUNTER} 

void spawn_asset(ros::NodeHandle n, geometry_msgs::Pose pose, int clr, bool k) {
//Initialise Spawn Model Variable
//Service Client type lets nodes access other node data through services
    ros::ServiceClient spawnModel=n.serviceClient<gazebo_msgs::SpawnModel>("gazebo/spawn_sdf_model");
//service client is type spawn model from gazebo message library. the spawn type defined is of an sdf model

//Spawn Model waits for spawn_sdf_model service to be avaliable for execution   
    spawnModel.waitForExistence();

//SpawnModel variable will hold the spawn data needed (name,xml,initial pose,referance)     
    gazebo_msgs::SpawnModel srv;
//Geometric File (as either sdf or urdf definition)
    std::ifstream ifs;
//Set SpawnModel parameters

if (k)
{
  srv.request.model_name = "Grid";
  ifs.open("src/armee_world/models/Grid/model.sdf");  
}

else
{
//Name of object that will be seen in Gazebo
    std::stringstream cni; //Stringstream item
    cni << count[0];       //Load Counter number into item
    std::string cns;        
    cni >> cns;
    std::string counter="Counter ";
    srv.request.model_name = counter+cns; //Gazebo Model name "Counter i"

    if (!clr && count[1]<25)
    {
        ifs.open("src/armee_world/models/RCounter/model.sdf");
    }
    else
    {
        ifs.open("src/armee_world/models/YCounter/model.sdf");
    }
}


 //load sdf whole file into variable sdf
std::stringstream buffer;
buffer << ifs.rdbuf(); //loading in text from input file through ifs variable
srv.request.model_xml = buffer.str();
//heirarchy within gazebo world ("/" for top)
srv.request.robot_namespace ="/";
//Inital robot pose
srv.request.initial_pose = pose;
//Reference frame of object pose
srv.request.reference_frame = "world";

spawnModel.call(srv); //server variable calles spawn request variable
}


int main(int argc, char **argv) {

    ros::init(argc, argv, "Enviro_cpp");
    ros::NodeHandle n;
//Initialise Gazebo spawn location parameters  
//Define cube position (m)
    geometry_msgs::Pose pose;
    pose.position.x = 1.200;
    pose.position.y = 0.000;
    pose.position.z = 0.978;
//Defining cube orientation (radians)
    tf2::Quaternion myQuaternion;
    myQuaternion.setRPY( 0, 0, 1.570796 );
//importing orientation to pose variable (must convert to message type)
    pose.orientation = tf2::toMsg(myQuaternion);
    spawn_asset(n,pose,0,1);
    
    myQuaternion.setRPY( 0, 0, 0 );
    pose.orientation = tf2::toMsg(myQuaternion);
    std::cout << "Generating Grid and Counters " << std::endl;
    pose.position.z = 0.82;
    for (int i = 0; i < 7; i++)
    {
        pose.position.x = 1.035+0.055*i;
        for (int j = 0; j < 7; j++)
        {
            pose.position.y = -0.755+0.055*j;
            count[0]++;
            int clr=rand() % 2;
            if (!clr)
            {
                count[1]++;
            }
            else
            {
                count[2]++;
            }
            
            spawn_asset(n,pose,clr,0);
            
        }
        
        
    }
    std::cout << "Red Counters: " << count[1] << std::endl;
    std::cout << "Yellow Counters: " << count[2] << std::endl;
    //spawn_box(n,pose);
   
    return 0;
}
