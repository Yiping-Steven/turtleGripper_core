#include <marker_tf_broadcast/broadcaster.hpp>
#include<tf/transform_broadcaster.h>
#include<tf/transform_listener.h>
#include<geometry_msgs/PoseStamped.h>
marker_tf::broadcaster::broadcaster( ros::NodeHandle& nh ):
  nh( nh ){
  nh.getParam( "marker_name", t_name );
  nh.getParam( "world_name", world_name );
  sub = nh.subscribe("/"+t_name+"/pose", 10, &broadcaster::callback, this);
  }

void marker_tf::broadcaster::callback( const geometry_msgs::PoseStamped& msg ){
  std::string world_name;
  std::string t_name;
  nh.getParam( "marker_name", t_name );
  nh.getParam( "world_name", world_name );
  poseStampedMsgToTF (msg, marker_tf_pose);
  tf::StampedTransform  marker_tf(marker_tf_pose, ros::Time::now(), world_name, t_name);
  bc.sendTransform( marker_tf );
}
