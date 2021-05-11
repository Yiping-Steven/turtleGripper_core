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

#include <tgrip_manip_srvcli/client.hpp>

// Called once when the goal completes
void doneCb(const actionlib::SimpleClientGoalState& state,
            const tgrip_manip_actions::pick_placeResultConstPtr& result)
{
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
  // ROS_INFO("Answer: %i", result->feedback_code.back());
  // ros::shutdown();
}

// Called once when the goal becomes active
void activeCb()
{
  ROS_INFO("Goal just went active");
}

// Called every time feedback is received for the goal
void feedbackCb(const tgrip_manip_actions::pick_placeFeedbackConstPtr& feedback)
{
    // ROS_INFO("Got Feedback of length %lu", feedback->feedback_code.size());
    // ROS_INFO("%s",feedback->feedback.feedback_code.c_str());

}

void clientCallback(const std_msgs::Float64MultiArray& position)
{
    actionlib::SimpleActionClient<tgrip_manip_actions::pick_placeAction> ac("action_server", true);

   
    ac.waitForServer();

    std_msgs::Float64MultiArray position;
    position.layout.dim.push_back(std_msgs::MultiArrayDimension());
    position.layout.dim[0].label = "postion";
    position.layout.dim[0].size = 3;
    position.layout.dim[0].stride = 1;
    position.layout.data_offset = 0;
    float xp = 0.2;
 
    position.data.push_back(position[0]);//x
    position.data.push_back(position[1]);//y
    position.data.push_back(position[0]);//z
    
    //std::cout << position << std::endl;
    

    tgrip_manip_actions::pick_placeGoal goal;
    goal.goal.position = position;

    for(int i=0;i<1;i++){

        ac.sendGoal(goal, &doneCb, &activeCb, &feedbackCb);
        
        bool res = ac.waitForResult( ros::Duration(30.0));

        actionlib::SimpleClientGoalState state = ac.getState();
        
        std::cout << state.toString().c_str() << std::endl;
        std::string result_s = state.toString();

    }
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "custom_planning");
    ros::NodeHandle nh;
    ros::AsyncSpinner spinner(1);
    spinner.start();
    sleep(2.0);

    ros::Subscriber sub =nh.subscribe("desired", 10, clientCallback)

    ros::spin();

    return 0;
}