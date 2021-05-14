
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_datatypes.h>

#include <boost/thread.hpp>

#include <sstream>
#include <string>
using namespace std;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

void spinThread(){
  ros::spin();
}

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle n;
  boost::thread spin_thread = boost::thread(boost::bind(&spinThread));
  MoveBaseClient ac("move_base_simple");
  //give some time for connections to register
  sleep(2.0);

  geometry_msgs::PoseStamped goalPose;
  move_base_msgs::MoveBaseGoal goal;
  move_base_msgs::MoveBaseActionGoal actionGoal;

  // user input
  goalPose.header.frame_id = "odom";
  goalPose.header.stamp = ros::Time::now();
  if (argc > 2){ // command line input
    string xStr = string(argv[1]);
    string yStr = string(argv[2]);
    string thetaStr = string(argv[3]);
    double theta;
    istringstream(xStr) >> goalPose.pose.position.x;
    istringstream(yStr) >> goalPose.pose.position.y;
    istringstream(thetaStr) >> theta;
    goalPose.pose.orientation = tf::createQuaternionMsgFromYaw(theta);
  } 
  else{ // the default pose, corresponding to the door location in the map_sim
    goalPose.pose.position.x = 7.25191450119;
    goalPose.pose.position.y = -7.33160924911;
    goalPose.pose.orientation = tf::createQuaternionMsgFromYaw(M_PI);
  }

  goal.target_pose = goalPose;

  actionGoal.goal = goal;
  actionGoal.header.frame_id = "odom";
  actionGoal.header.stamp = ros::Time::now();
  actionGoal.goal_id.stamp = ros::Time::now();
  actionGoal.goal_id.id = 2333;


  ROS_INFO("Sending goal");
  ac.sendGoal(actionGoal.goal);

  // ros::Publisher goalPub =  n.advertise<geometry_msgs::PoseStamped>
  //                               ("/move_base_simple/goal",10);                              
  // goalPub.publish(goalPose); //MoveBaseActionGoal

  ros::Publisher goalPub =  n.advertise<move_base_msgs::MoveBaseActionGoal>
                                ("/move_base_simple/goal",10);                                
  goalPub.publish(actionGoal); //MoveBaseActionGoal

  // ros::Publisher goalPub =  n.advertise<move_base_msgs::MoveBaseGoal>
  //                               ("/move_base_simple/goal",10);
  // goalPub.publish(goal); //MoveBaseGoal

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the base moved 2 meters forward");
  else
    ROS_INFO("The base failed to move forward 2 meters for some reason");

  return 0;
}
