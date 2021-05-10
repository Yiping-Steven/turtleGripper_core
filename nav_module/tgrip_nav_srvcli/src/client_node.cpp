#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <rsp_turtlebot_actions/moveAction.h>
// #include <rsp_turtlebot_actions/moveFeedback.h>
#include <sstream>
#include <string>

using namespace std;


// void feedback( const moveFeedback& feedback){
//     ROS_INFO("Feedback Message: %s", feedback->c_str() );
// }

int main( int argc, char** argv ){
    ros::init( argc, argv, "client");
    ros::NodeHandle nh;

    actionlib::SimpleActionClient<rsp_turtlebot_actions::moveAction> ac("action_server", true);
    ac.waitForServer();

    rsp_turtlebot_actions::moveGoal goal1, goal2;
    goal1.goal.goalCode = std::string("A new goal");

    string xStr = string(argv[1]);
    string yStr = string(argv[2]);
    string thetaStr = string(argv[3]);
    string posTolStr = string(argv[4]);
    string angTolStr = string(argv[5]);

    istringstream(xStr) >> goal1.goal.poseGoal.x;
    istringstream(yStr) >> goal1.goal.poseGoal.y;
    istringstream(thetaStr) >> goal1.goal.poseGoal.theta;
    istringstream(angTolStr) >> goal1.goal.angTol;
    istringstream(posTolStr) >> goal1.goal.posTol;

    goal2 = goal1;
    goal2.goal.poseGoal.x+=1;
    

    bool res1;
    ac.sendGoal( goal1 );
    res1 = ac.waitForResult( ros::Duration(30.0) );

    if (res1) {
        actionlib::SimpleClientGoalState state = ac.getState();
        std::cout << state.toString().c_str() << std::endl;
    }



    // bool res2;
    // ac.sendGoal( goal2 );
    // res2 = ac.waitForResult( ros::Duration(30.0) );

    // if (res2) {
    //     actionlib::SimpleClientGoalState state = ac.getState();
    //     std::cout << state.toString().c_str() << std::endl;
    // }


    // ros::spin(); //waiting there.

    return 0;
}