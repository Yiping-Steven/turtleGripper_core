#include <ros/ros.h>
#include <tgrip_manip_actions/pick_placeAction.h>
#include <actionlib/server/simple_action_server.h>
#include <geometry_msgs/Twist.h>
#include <memory>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/JointState.h>

//MOVE IT
#include <iostream>
#include <stdlib.h> 
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
//Open
#include "open_manipulator_msgs/KinematicsPose.h"
#include "open_manipulator_msgs/SetJointPosition.h"
#include "open_manipulator_msgs/SetKinematicsPose.h"
#include "open_manipulator_msgs/SetDrawingTrajectory.h"
#include "open_manipulator_msgs/SetActuatorState.h"
#include "open_manipulator_msgs/GetJointPosition.h"
#include "open_manipulator_msgs/GetKinematicsPose.h"
#include "open_manipulator_msgs/OpenManipulatorState.h"

class server
{
    typedef actionlib::SimpleActionServer<tgrip_manip_actions::pick_placeAction> ActionServer;

private:
    /* data */
    ros::NodeHandle nh;
    ros::Subscriber sub_vw;
    ros::Subscriber sub_js;
    ros::Publisher pub_qd;  
    
    std::unique_ptr<ActionServer> as;


public:
    int stage = 0;
    server(ros::NodeHandle &nh);
    //~rsp_server();
    void goalCB();
    void preemptCB();
    void callback_p(const sensor_msgs::JointState &current_position);

};

