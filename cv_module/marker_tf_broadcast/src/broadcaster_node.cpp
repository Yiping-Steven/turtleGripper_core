#include <marker_tf_broadcast/broadcaster.hpp>

int main( int argc, char** argv ){

  ros::init( argc, argv, "broadcaster_node" );
  ros::NodeHandle nh;
  ros::NodeHandle nhp("~");
  
  std::string world_name;
  std::string t_name;
  nh.getParam( "marker_name", t_name );
  nh.getParam( "world_name", world_name );
  marker_tf::broadcaster broadcaster( nh );

  ros::spin();

  return 0;

}
