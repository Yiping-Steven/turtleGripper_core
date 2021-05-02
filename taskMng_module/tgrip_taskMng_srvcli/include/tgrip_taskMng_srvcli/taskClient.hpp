#include <ros/ros.h>
#include <std_msgs/String.h>
#include <tgrip_taskMng_msgs/serviceQuery.h>

namespace tgrip{

  class tskMngCliClass{
  
  private: 

    ros::NodeHandle nh;

  public:

    tskMngCliClass( ros::NodeHandle& nh);
    ~tskMngCliClass();

    ros::ServiceClient client;

  };

}