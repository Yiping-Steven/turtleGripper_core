#include <ros/ros.h>

#include <tgrip_manip_actions/pick_placeAction.h>
#include <tgrip_manip_actions/pick_placeFeedback.h>
#include <actionlib/client/simple_action_client.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>

#include <memory>
#include <boost/function.hpp>

class client
{
    typedef actionlib::SimpleActionClient<tgrip_manip_actions::pick_placeAction> ActionClient;
    //typedef boost::function<void (const rsp_turtlebot_actions::pick_placeActionFeedbackPtr& feedback)> feedbackCb;
   
private:

    ros::NodeHandle nh;
    ros::Subscriber sub_cube;

    std::unique_ptr<ActionClient> ac;

public:

    client(ros::NodeHandle &nh);
    void sendCb(const geometry_msgs::Point& point);

};

