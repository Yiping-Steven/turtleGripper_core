#include <ros/ros.h>
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


int main(int argc, char **argv)
{

  ros::init(argc, argv, "custom_planning");
  ros::NodeHandle nh;
  ros::AsyncSpinner spinner(1);
  spinner.start();
  // sleep(2.0);

    // double x =atof(argv[0]);
    // double y =atof(argv[1]);
    // double z =atof(argv[2]);


//planning group that we would like to control
  moveit::planning_interface::MoveGroupInterface group("arm");
  moveit::planning_interface::MoveGroupInterface group_g("gripper");

  const robot_state::JointModelGroup *joint_model_group = group.getCurrentState()->getJointModelGroup("arm");

  // const robot_state::JointModelGroup* gripper_model_group =
  //     group_g.getCurrentState()->getJointModelGroup("gripper");

  // group.setPlannerId("RRTConnect");
  //raw pointers are used to refer to the planning group for improved performance
  // group.setEndEffectorLink("link5");
  moveit::core::RobotStatePtr current_state = group.getCurrentState();
  // group.setStartStateToCurrentState();
  //const std::string &end_effector_link= "end_effector_link";

// home joint(0 0.879 0.290 -1.180)
/*joint*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////
  std::vector<double> joint_group_positions;
  current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);
  
  joint_group_positions[0] = -0.8;  // radians
  joint_group_positions[1] = 0.4;  // radians
  joint_group_positions[2] = 0.8;  // radians
  joint_group_positions[3] = -0.1;  // radians

  group.setJointValueTarget(joint_group_positions);
  group.setGoalTolerance(0.05);

  // moveit::planning_interface::MoveGroupInterface::Plan my_plan;

  // bool success = (group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

  // ROS_INFO_NAMED("tutorial", "move joint", success ? "" : "FAILED");

  // if (!success)
  //   throw std::runtime_error("No plan found");
  group.move(); //blocking
  sleep(5.0);

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // group_g.getCurrentState();
  // group_g.setStartStateToCurrentState();

  // std::vector<double> gripper_value;
  // gripper_value.push_back(0.1);

  // group_g.setJointValueTarget(gripper_value);

  // success = (group_g.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
  // ROS_INFO_NAMED("tutorial", "gripper", success ? "" : "FAILED");

  // if (!success)
  //   throw std::runtime_error("No plan found");
  // group_g.move(); //blocking
  // sleep(5.0);

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ros::shutdown();

  return 0;
}

/* pose 

    geometry_msgs::Pose target_pose1;

    target_pose1.position.x = 0.27;
    target_pose1.position.y = 0;
    target_pose1.position.z = 0;
    target_pose1.orientation.w = 0.938138224541;
    target_pose1.orientation.x = 0.000000;
    target_pose1.orientation.y = 0.346260987776;
    target_pose1.orientation.z = 0.000000;

    group.setPositionTarget(target_pose1.position.x,  target_pose1.position.y, target_pose1.position.z, "link5");
    group.setApproximateJointValueTarget(target_pose1,"link5");// only works with this function
    group.setGoalTolerance(0.05);

    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    moveit::planning_interface::MoveItErrorCode success = group.plan(my_plan);
    ROS_INFO("Visualizing plan 1 (pose goal) %s", success.val ? "" : "FAILED");

*/