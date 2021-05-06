#include <om_test/sub.hpp>
/*
void chatterCallback(const sensor_msgs::JointState& joint)
{
  std::cout << joint << std::endl;
}
*/
int main(int argc, char** argv){

    ros::init(argc, argv, "om_sub");
    ros::NodeHandle nh;

    
    sub sub(nh);
    //ros::Subscriber sub = nh.subscribe("/joint_states", 1000, chatterCallback);
    ros::spin();

    return 0;

}