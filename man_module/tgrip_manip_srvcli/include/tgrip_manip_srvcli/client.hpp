#include <ros/ros.h>
#include <tgrip_manip_actions/pick_placeAction.h>
#include <tgrip_manip_actions/pick_placeFeedback.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Twist.h>
#include <memory>
#include <nav_msgs/Odometry.h>
#include <boost/function.hpp>

class client
{
    typedef actionlib::SimpleActionClient<tgrip_manip_actions::pick_placeAction> ActionClient;
    //typedef boost::function<void (const rsp_turtlebot_actions::pick_placeActionFeedbackPtr& feedback)> feedbackCb;
   
private:
    /* data */
    ros::NodeHandle nh;
    ros::Subscriber sub_vw;
    ros::Subscriber sub_js;
    ros::Publisher pub_qd;
    ros::Subscriber sub_fb;

    std::unique_ptr<ActionClient> ac;

public:

    client(ros::NodeHandle &nh);
};

