#include <marker_tf_broadcast/broadcaster.hpp>

int main( int argc, char** argv ){

  ros::init( argc, argv, "broadcaster_node" );
  ros::NodeHandle nh;
  ros::NodeHandle nhp("~");
  marker_tf::broadcaster broadcaster( nh );

  ros::spin();

  return 0;

}
