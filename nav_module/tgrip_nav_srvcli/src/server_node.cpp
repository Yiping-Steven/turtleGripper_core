#include <rsp_turtlebot_srvcli/server.hpp>

int main (int argc, char** argv){
    ros::init( argc, argv, "action_srv");
    ros::NodeHandle nh;

    rsp_turtlebot_server server(nh);

    // ros::spin();
    ros::MultiThreadedSpinner spinner(2);
    spinner.spin();

    return 0;
}