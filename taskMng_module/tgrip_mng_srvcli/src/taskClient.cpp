#include <tgrip_mng_srvcli/taskClient.hpp>

tgrip::tskMngCliClass::tskMngCliClass( ros::NodeHandle& nh) {

  client = nh.serviceClient< tgrip_mng_msgs::serviceQuery >("tgrip_mng_service");

}

tgrip::tskMngCliClass::~tskMngCliClass(){}