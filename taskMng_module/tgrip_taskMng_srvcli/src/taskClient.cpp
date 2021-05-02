#include <tgrip_taskMng_srvcli/taskClient.hpp>

tgrip::tskMngCliClass::tskMngCliClass( ros::NodeHandle& nh) {

  client = nh.serviceClient< tgrip_taskMng_msgs::serviceQuery >("serviceQuery");

}

tgrip::tskMngCliClass::~tskMngCliClass(){}