#include <om_test/sub.hpp>

sub::sub( ros::NodeHandle& nh):
    nh(nh){
    std::cout << "here" << std::endl;

    subs = nh.subscribe("joint_states", 10, &sub::poseCallback, this );
    sub_tf= nh.subscribe("tf", 10, &sub::tfCallback, this );


}

sub::~sub(){}

void sub::poseCallback(const sensor_msgs::JointState::ConstPtr& msg){
  std::vector<double> temp_angle;
  temp_angle.resize(NUM_OF_JOINT_AND_TOOL);
  for(int i = 0; i < msg->name.size(); i ++)
  {
    if(!msg->name.at(i).compare("joint1"))  temp_angle.at(0) = (msg->position.at(i));
    else if(!msg->name.at(i).compare("joint2"))  temp_angle.at(1) = (msg->position.at(i));
    else if(!msg->name.at(i).compare("joint3"))  temp_angle.at(2) = (msg->position.at(i));
    else if(!msg->name.at(i).compare("joint4"))  temp_angle.at(3) = (msg->position.at(i));
    else if(!msg->name.at(i).compare("gripper"))  temp_angle.at(4) = (msg->position.at(i));
  }
  present_joint_angle_ = temp_angle;

    for(int i=0; i<present_joint_angle_.size(); ++i){
     std::cout << "joint"<<i<<" "<<present_joint_angle_[i] << std::endl;
    }
}
void sub::tfCallback(const tf2_msgs::TFMessageConstPtr& tf){

  std::cout << "here2" << std::endl;

}