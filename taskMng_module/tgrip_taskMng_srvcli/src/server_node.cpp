#include <tgrip_taskMng_srvcli/taskServer.hpp>

int main (int argc, char** argv){
    ros::init( argc, argv, "tgrip_TaskMng_Server");
    ros::NodeHandle nh;

    tgrip_taskMng_server server(nh);

    // ros::spin();
    ros::MultiThreadedSpinner spinner(2);
    spinner.spin();

    return 0;
}