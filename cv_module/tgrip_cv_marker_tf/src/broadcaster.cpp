#include<tf/transform_broadcaster.h>
#include<tf/transform_listener.h>
#include<ros/ros.h>
#include<string>
#include<geometry_msgs/PoseStamped.h>

void callback(const geometry_msgs::PoseStamped& msg){
  //Callback broadcasts the TF of the marker

  ros::NodeHandle nhp("~");
  tf::TransformBroadcaster bc;
  tf::Stamped<tf::Pose> marker_tf_pose;
  std::string world_name;
  std::string t_name;
  nhp.getParam( "marker_name", t_name );
  nhp.getParam( "world_name", world_name );

  poseStampedMsgToTF (msg, marker_tf_pose);
  tf::StampedTransform  marker_tf(marker_tf_pose, ros::Time::now(), world_name, t_name);
  bc.sendTransform( marker_tf );
}

int main( int argc, char** argv ){

  ros::init( argc, argv, "broadcaster_node" );
  ros::NodeHandle nh;
  ros::NodeHandle nhp("~");
  ros::Rate rate( 10.0 );
  ros::Subscriber sub;
  std::string t_name;
  nhp.getParam( "marker_name", t_name );
  
  sub = nh.subscribe("/"+t_name+"/pose", 10, &callback);
  ros::spin();

  return 0;

}
