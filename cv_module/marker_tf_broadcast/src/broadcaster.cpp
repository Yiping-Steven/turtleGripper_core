#include <marker_tf_broadcast/broadcaster.hpp>

marker_tf::broadcaster::broadcaster( ros::NodeHandle& nh ):
  nh( nh ){
  sub = nh.subscribe("/"+t_name+"/pose", 10, &broadcaster::callback, this);
  }

void marker_tf::broadcaster::callback( const geometry_msgs::PoseStamped& msg ){

  poseStampedMsgToTF (msg, marker_tf_pose);
  tf::StampedTransform  marker_tf(marker_tf_pose, ros::Time::now(), world_name, t_name);
  bc.sendTransform( marker_tf );
}
