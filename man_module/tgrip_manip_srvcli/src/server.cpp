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
   
    group.setPlannerId("RRTConnectkConfigDefault");
    group.setEndEffectorLink("link5");
    group.getCurrentState();
    group.setStartStateToCurrentState();
    // std::cout<<po.pose.pose<<std::endl;
    // geometry_msgs::Twist pub_topic;

    // //std::cout<<po.pose.pose.position.x<<std::endl;
    tgrip_manip_actions::pick_placeGoal goal = *(as->acceptNewGoal());
    // std::cout<<"get client "<<"  "<<goal.goal<<std::endl;
    
    // float tol_p = goal.goal.position_error;
    // float tol_a = goal.goal.angular_error;

    float new_x = goal.goal.position.data[0];
    float new_y = goal.goal.position.data[1];
    float new_z = goal.goal.position.data[2];

    geometry_msgs::Pose target_pose1;

    target_pose1.position.x = new_x;//0.124;
    target_pose1.position.y = new_y;//0;
    target_pose1.position.z = new_z;//0.3;

    group.setPositionTarget(target_pose1.position.x,  target_pose1.position.y, target_pose1.position.z, "link5");
    group.setApproximateJointValueTarget(target_pose1,"link5");// only works with this function
    group.setGoalTolerance(0.05);


    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    moveit::planning_interface::MoveItErrorCode success = group.plan(my_plan);
    ROS_INFO("Visualizing plan 1 (pose goal) %s", success.val ? "" : "FAILED");
//If cannot find plan
    if (!success)
        throw std::runtime_error("No plan found");
 //If find plan      
    group.move(); //blocking
    sleep(5.0);

        

  std::vector<double> gripper_value;
  gripper_value.push_back(-0.1);
//open +0.1
  group_g.setJointValueTarget(gripper_value);

  success = (group_g.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
  ROS_INFO_NAMED("tutorial", "gripper", success ? "" : "FAILED");

  if (!success)
      throw std::runtime_error("No plan found");
  group_g.move(); //blocking
  sleep(5.0);
  ROS_INFO("set tool control");


}

void server::preemptCB(){

    
}