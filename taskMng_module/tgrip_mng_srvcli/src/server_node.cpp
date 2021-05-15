#include <tgrip_mng_srvcli/taskManager.hpp>

int main (int argc, char** argv){

    ros::init( argc, argv, "tgrip_mng_server");
    ros::NodeHandle nh;

    tgrip::taskManager server(nh);

    ros::spin();
    ros::MultiThreadedSpinner spinner(2);
    spinner.spin();

    return 0;
}