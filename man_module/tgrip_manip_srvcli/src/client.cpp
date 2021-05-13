#include <tgrip_manip_srvcli/client.hpp>
#include <cmath>

client::client(ros::NodeHandle& nh): nh(nh){

    
    sub_cube = nh.subscribe("/cube_position", 10, &client::sendCb, this); //geometry_msgs::point


}

void client::sendCb(const geometry_msgs::Point& point)
{

    actionlib::SimpleActionClient<tgrip_manip_actions::pick_placeAction> ac("action_server", true);

    ac.waitForServer();

    geometry_msgs::Point cube_point;
    cube_point.x = point.x;
    cube_point.y = point.y;
    cube_point.z = point.z;

    // std::cout<<cube_point<<std::endl;

    geometry_msgs::Pose pose;

    
//0.430  0.015 0
//0.417 0.0 0.1
    pose.position.x = cube_point.x;
    pose.position.y = cube_point.y;
    pose.position.z = cube_point.z;
    pose.orientation.x = 0.0;
    pose.orientation.y = 0.0;
    pose.orientation.z = 0.0;
    pose.orientation.w = 1;

    tgrip_manip_actions::pick_placeGoal goal;

    goal.goal.pose = pose;

    // std::cout<<pose<<std::endl;
  
  ac.sendGoal(goal);
        
  bool res = ac.waitForResult( ros::Duration(30.0));

  actionlib::SimpleClientGoalState state = ac.getState();
        
  std::cout << state.toString().c_str() << std::endl;
  std::string result_s = state.toString();

}
