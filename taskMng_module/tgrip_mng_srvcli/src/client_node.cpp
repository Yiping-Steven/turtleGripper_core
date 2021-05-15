#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <tgrip_mng_srvcli/taskClient.hpp>
// #include <tgrip_mng_actions/moveAction.h>
// #include <tgrip_taskmng_actions/moveFeedback.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

string taskNameArray[] = {"mapping", "fetching", "timedPatrol", "meteredPatrol"};
string taskName;

int main( int argc, char** argv ){
    ros::init( argc, argv, "tgrip_mng_client");
    ros::NodeHandle nh;
    tgrip::tskMngCliClass client(nh);
    tgrip_mng_msgs::serviceQuery taskInitSrv;

    while(ros::ok()){

        // User interface
        std::cout<< "Dear User, turtleGripper is at your service"<<std::endl
                << "please enter the command code: " << std::endl
                << "1. Mapping. " << std::endl 
                << "2. Fetching." << std::endl
                << "3. Patrol for a period. " << std::endl
                << "4. Patrol for a distance." << std::endl;

        // Interpreting input
        string cmdStr;
        std::cin>> cmdStr;
        int cmdInt = stoi(cmdStr);
        taskName = taskNameArray[cmdInt];

        // Sending request
        taskInitSrv.request.queryStr = taskName;
        taskInitSrv.request.queryCode = cmdInt;

        if (client.client.call(taskInitSrv))
        {
            ROS_INFO("Response from the server: %s", taskInitSrv.response.replyStr.c_str());
        }
        else
        {
            ROS_ERROR("Failed to call the service 111");
            return 1;
        }
    }
    return 0;
}