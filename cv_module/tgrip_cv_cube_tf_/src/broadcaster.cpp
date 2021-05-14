#include<tf/transform_broadcaster.h>
#include<tf/transform_listener.h>
#include<ros/ros.h>
#include<string>
#include<geometry_msgs/PoseStamped.h>


void callback(const geometry_msgs::PoseStamped& msg){
  //Callback calculates pose of the cube by translating marker pose along y axis
  
  ros::NodeHandle nhp("~");
  tf::TransformBroadcaster bc;
  tf::Transform transform;
  std::string world_name;
  std::string marker_name;
  nhp.getParam( "marker_name", marker_name );
  

  transform.setOrigin( tf::Vector3(0.0, -0.028, 0.0) );
  transform.setRotation( tf::Quaternion(0, 0, 0, 1) );
  tf::TransformListener listener;
  ros::Time now = ros::Time::now();
  listener.waitForTransform("base_link", marker_name, now, ros::Duration(0.1));
  bc.sendTransform( tf::StampedTransform( transform, now, marker_name, "cube" ));
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
