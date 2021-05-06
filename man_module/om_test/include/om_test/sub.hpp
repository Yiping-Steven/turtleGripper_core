#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf2_msgs/TFMessage.h>

#include "open_manipulator_msgs/OpenManipulatorState.h"
#include "open_manipulator_msgs/SetJointPosition.h"
#include "open_manipulator_msgs/SetKinematicsPose.h"
#include "open_manipulator_msgs/SetDrawingTrajectory.h"
#include "open_manipulator_msgs/SetActuatorState.h"

#define NUM_OF_JOINT_AND_TOOL 5

class sub{

private:

    ros::NodeHandle nh;
    ros::Subscriber subs;
    ros::Subscriber sub_tf;

public:

    sub(ros::NodeHandle& nh);
    ~sub();

    void poseCallback(const sensor_msgs::JointState::ConstPtr& joint);
    void tfCallback(const tf2_msgs::TFMessageConstPtr& tf);
    std::vector<double> present_joint_angle_;


};