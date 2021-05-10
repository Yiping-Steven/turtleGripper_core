#include <ros/ros.h>
#include <tgrip_nav_actions/moveAction.h>
#include <actionlib/server/simple_action_server.h>
#include <memory>

#include <nav_msgs/Odometry.
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>
#include <tf/tf.h>
#include <time.h>
#include <math.h>
#include <string>

#define PI 3.1415926

class tgrip_nav_server{

    typedef actionlib::SimpleActionServer<move_base::MoveBaseAction> ActionServer;

private:

    ros::NodeHandle nh;
    std::unique_ptr<ActionServer> as;
    ros::Subscriber subOdom;
    ros::Publisher pubCmd;
    geometry_msgs::Pose2D poseCurrent; //real time state of the robot

    tgrip_nav_actions::moveGoal goal;
    tgrip_nav_actions::moveFeedback feedback;
    tgrip_nav_actions::moveResult result ;

    // Time control
    double angSpeedTol, posSpeedTol;
    time_t timerMissionStart; // unit in seconds
    time_t timerPhase1;
    time_t timerPhase2;
    time_t timerPhase3;
 

public:

    tgrip_nav_server(ros::NodeHandle& nh);

    void goalCB();
    void odomCallback(const nav_msgs::Odometry::ConstPtr& odomMsgPtr);
    void publish();

};