#include <tgrip_manip_srvcli/server.hpp>
#include <cmath>


server::server(ros::NodeHandle& nh): nh(nh){
    // sub_js = nh.subscribe("/joint_states", 10, &server::callback_p, this);
    // pub_qd = nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop",10);
        
    as.reset(new ActionServer(nh, "action_server", false));

    as->registerGoalCallback(boost::bind(&server::goalCB, this));
    as->registerPreemptCallback(boost::bind(&server::preemptCB, this));

    as->start();
    
}

void server::callback_p(const sensor_msgs::JointState &current_position){
    // po = current_position;

}

void server::goalCB(){
    
    moveit::planning_interface::MoveGroupInterface group("arm");
    moveit::planning_interface::MoveGroupInterface group_g("gripper");
   
    // group.setPlannerId("RRTConnect");
    group.setEndEffectorLink("end_effector_link");
    group.getCurrentState();
    group.setStartStateToCurrentState();
    // std::cout<<po.pose.pose<<std::endl;
    // geometry_msgs::Twist pub_topic;

    // //std::cout<<po.pose.pose.position.x<<std::endl;
    tgrip_manip_actions::pick_placeGoal goal = *(as->acceptNewGoal());
    // std::cout<<"get client "<<"  "<<goal.goal<<std::endl;
    
    // float tol_p = goal.goal.position_error;
    // float tol_a = goal.goal.angular_error;

    // float new_x = goal.goal.position.data[0];
    // float new_y = goal.goal.position.data[1];
    // float new_z = goal.goal.position.data[2];
///////////////////////////////////////////////////////////////////////////////////move to z offset     

    geometry_msgs::Pose target_pose1;

    target_pose1.position.x = goal.goal.pose.position.x;//0.124;
    target_pose1.position.y = goal.goal.pose.position.y;//0;
    target_pose1.position.z = goal.goal.pose.position.z+0.15;//0.3;
    target_pose1.orientation.x = goal.goal.pose.orientation.x;
    target_pose1.orientation.y = goal.goal.pose.orientation.y;
    target_pose1.orientation.z = goal.goal.pose.orientation.z;
    target_pose1.orientation.w = goal.goal.pose.orientation.w;


    // group.setPositionTarget(target_pose1.position.x,  target_pose1.position.y, target_pose1.position.z, "end_effector_link");
    group.setPoseTarget(target_pose1, "end_effector_link");
    group.setApproximateJointValueTarget(target_pose1,"end_effector_link");// only works with this function
    // group.setGoalTolerance(0.05);


    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    moveit::planning_interface::MoveItErrorCode success = group.plan(my_plan);
    // ROS_INFO("Visualizing plan 1 (pose goal) %s", success.val ? "" : "FAILED");
          //If cannot find plan
    if (!success)
      ROS_INFO("fail to move above");
          //If find plan      
    group.move(); //blocking
    sleep(5.0);

///////////////////////////////////////////////////////////////////////////////////move to real position        

//     geometry_msgs::Pose target_pose;

//     target_pose.position.x = new_x;//0.124;
//     target_pose.position.y = new_y;//0;
//     target_pose.position.z = new_z;//0.3;
//     target_pose.orientation.x = 0.0;
//     target_pose.orientation.y = 0.01;
//     target_pose.orientation.z = -0.03;
//     target_pose.orientation.w = 0.99;

//     group.setPoseTarget(target_pose, "end_effector_link");
//     // group.setPositionTarget(target_pose.position.x,  target_pose.position.y, target_pose.position.z, "end_effector_link");
//     group.setApproximateJointValueTarget(target_pose,"end_effector_link");// only works with this function
//     // group.setGoalTolerance(0.05);


//     success = group.plan(my_plan);
//     // ROS_INFO("Visualizing plan 2 (pose goal) %s", success.val ? "" : "FAILED");
//           //If cannot find plan
//     if (!success)
//       ROS_INFO("fail to move point");
//         // throw std::runtime_error("No plan found");
//           //If find plan      
//     group.move(); //blocking
//     sleep(5.0);
// ///////////////////////////////////////////////////////////////////////////////////pick      

//   std::vector<double> gripper_value;
//   gripper_value.push_back(0.01);
// //open +0.1
//   group_g.setJointValueTarget(gripper_value);

//   success = (group_g.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
//   // ROS_INFO_NAMED("tutorial", "gripper", success ? "" : "FAILED");

//   if (!success)
//       ROS_INFO("fail to pick");
//   else
//     group_g.move(); //blocking
//     sleep(5.0);
//     ROS_INFO("pick");

// ///////////////////////////////////////////////////////////////////////////////////
//     group.setPositionTarget(target_pose1.position.x,  target_pose1.position.y, target_pose1.position.z, "end_effector_link");
//     group.setApproximateJointValueTarget(target_pose1,"end_effector_link");// only works with this function
//     group.setGoalTolerance(0.05);
    
//     success = group.plan(my_plan);
//     // ROS_INFO("Visualizing plan 1 (pose goal) %s", success.val ? "" : "FAILED");
//           //If cannot find plan
//     if (!success)
//       ROS_INFO("fail to return above");
//           //If find plan      
//     else
//       group.move(); //blocking
//       sleep(5.0);

// ///////////////////////////////////////////////////////////////////////////////////move to home      
//   const robot_state::JointModelGroup *joint_model_group = group.getCurrentState()->getJointModelGroup("arm");
//   moveit::core::RobotStatePtr current_state = group.getCurrentState();
//   std::vector<double> joint_group_positions;
//   current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);
  
//   joint_group_positions[0] = -0;  // radians
//   joint_group_positions[1] = 0;  // radians
//   joint_group_positions[2] = 0;  // radians
//   joint_group_positions[3] = -0;  // radians

//   group.setJointValueTarget(joint_group_positions);
//   group.setGoalTolerance(0.05);

//   group.move(); //blocking
//   // sleep(5.0);
//   ROS_INFO("home");

// ///////////////////////////////////////////////////////////////////////////////////move to loco   
//   joint_model_group = group.getCurrentState()->getJointModelGroup("arm");
//   current_state = group.getCurrentState();

//   std::vector<double> joint_group_positions1;
//   current_state->copyJointGroupPositions(joint_model_group, joint_group_positions1);
  
//   joint_group_positions1[0] = -0.7;  // radians
//   joint_group_positions1[1] = 0.4;  // radians
//   joint_group_positions1[2] = 0.8;  // radians
//   joint_group_positions1[3] = -0.1;  // radians

//   group.setJointValueTarget(joint_group_positions1);
//   group.setGoalTolerance(0.05);

//   group.move(); //blocking
//   sleep(5.0);
//   ROS_INFO("home");  

}

void server::preemptCB(){

    
}
