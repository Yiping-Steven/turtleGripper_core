#include <tgrip_manip_srvcli/client.hpp>
#include <cmath>

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

client::client(ros::NodeHandle& nh): nh(nh){
    //sub_fb = nh.subscribe("/action_server/feedback", 10, &client::feedbackCb, this);

    actionlib::SimpleActionClient<tgrip_manip_actions::pick_placeAction> ac("action_server", true);

   
    ac.waitForServer();

    geometry_msgs::Pose pose;

    pose.position.x = 0.373;
    pose.position.y = -0.004;
    pose.position.z = 0.012;
    pose.orientation.x = 0.0;
    pose.orientation.y = 0.01;
    pose.orientation.z = -0.03;
    pose.orientation.w = 0.99;
    // std_msgs::Float64MultiArray position;
    // position.layout.dim.push_back(std_msgs::MultiArrayDimension());
    // position.layout.dim[0].label = "postion";
    // position.layout.dim[0].size = 3;
    // position.layout.dim[0].stride = 1;
    // position.layout.data_offset = 0;
    // float xp = 0.2;
 
    // position.data.push_back(0.4);//x
    // position.data.push_back(-0.2);//y
    // position.data.push_back(0.0);//z
    
    //std::cout << position << std::endl;
    

    tgrip_manip_actions::pick_placeGoal goal;
    goal.goal.pose = pose;

    for(int i=0;i<1;i++){

        ac.sendGoal(goal, &doneCb, &activeCb, &feedbackCb);
        
        bool res = ac.waitForResult( ros::Duration(30.0));

        actionlib::SimpleClientGoalState state = ac.getState();
        
        std::cout << state.toString().c_str() << std::endl;
        std::string result_s = state.toString();

    }
}
