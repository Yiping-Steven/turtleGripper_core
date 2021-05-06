#include <tgrip_manip_srvcli/server.hpp>

int main(int argc, char** argv){

    ros::init(argc, argv, "action_server");
    ros::NodeHandle nh;

    server server(nh);
    ros::MultiThreadedSpinner spinner(2);
    spinner.spin();

    return 0;
}