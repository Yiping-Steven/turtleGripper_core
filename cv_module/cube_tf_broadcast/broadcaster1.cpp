#include<tf/transform_broadcaster.h>
#include<tf/transform_listener.h>
#include<ros/ros.h>
#include<string>
#include<geometry_msgs/PoseStamped.h>

//#include<geometry_msgs/TransformStamped.h>

void callback(const geometry_msgs::PoseStamped& msg){
  //thank you to this tutorial http://wiki.ros.org/tf2/Tutorials/Writing%20a%20tf2%20broadcaster%20%28C%2B%2B%29

  ros::NodeHandle nhp("~");
  tf::TransformBroadcaster bc;
  tf::TransformListener listener;

  std::string world_name;
  std::string marker_name;
  std::string cube_name ("cube_name");
  nhp.getParam( "world_name", world_name );
  nhp.getParam( "marker_name", marker_name );
  //Create Pose of the cube's center relative to marker
  geometry_msgs::PoseStamped cube_center;
  
  std::cout<<"Callbalck"<<std::endl;
  
  cube_center.header.frame_id = "cube_center";
  cube_center.header.stamp = ros::Time();
  cube_center.pose.orientation.x = 0.0;
  cube_center.pose.orientation.y = 0.0;
  cube_center.pose.orientation.z = 0.0;
  cube_center.pose.orientation.w = 1.0;
  cube_center.pose.position.x = 0.0;
  cube_center.pose.position.y = 0.0;
  cube_center.pose.position.z = -0.028;

  std::cout<<cube_center<<std::endl;
  
  //Transform this pose to coordinates relative to the world
  listener.waitForTransform(world_name, marker_name, ros::Time::now(), ros::Duration(1));
  
  geometry_msgs::PoseStamped cube_pose;
  listener.transformPose(world_name, cube_center, cube_pose); 
  //Transform Pose to TF
  // tf::Stamped<tf::Pose> cube_tf_pose;
  // poseStampedMsgToTF (cube_pose, cube_tf_pose);
  // tf::StampedTransform  cube_tf(cube_tf_pose, ros::Time::now(), world_name, cube_name);
  
  // bc.sendTransform( cube_tf );
}

int main( int argc, char** argv ){

  ros::init( argc, argv, "broadcaster_node" );
  ros::NodeHandle nh;
  ros::NodeHandle nhp("~");
  ros::Rate rate( 10.0 );
  ros::Subscriber sub;
  std::string t_name;
  nhp.getParam( "marker_name", t_name );
  std::cout<<"Main"<<std::endl;
  sub = nh.subscribe("/"+t_name+"/pose", 10, &callback);
  ros::spin();

  return 0;

}
