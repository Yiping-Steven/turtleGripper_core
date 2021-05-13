#include <ros/ros.h>
#include <tgrip_manip_srvcli/client.hpp>
#include <tgrip_manip_actions/pick_placeAction.h>
#include <std_msgs/Float64MultiArray.h>



int main(int argc, char** argv){

    ros::init(argc, argv, "client");
    ros::NodeHandle nh;

    client client(nh);

    ros::MultiThreadedSpinner spinner(2);
    spinner.spin();
    
    return 0;
}