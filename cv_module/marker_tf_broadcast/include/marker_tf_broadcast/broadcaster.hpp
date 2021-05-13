#include<tf/transform_broadcaster.h>
#include<tf/transform_listener.h>
#include<ros/ros.h>
#include<string>
#include<geometry_msgs/PoseStamped.h>

namespace marker_tf{
 
  class broadcaster{
  // broadcaster subscribes to aruco_single_X and broadcasts its tf realtive to world_frame
    private: 


    public:
      ros::NodeHandle nh;
      std::string world_name;
      std::string t_name;
      ros::Subscriber sub; //subscribes to aruco_single_x/pose
      tf::Stamped<tf::Pose> marker_tf_pose;
      tf::TransformBroadcaster bc;

      broadcaster ( ros::NodeHandle& nh);
      void callback( const geometry_msgs::PoseStamped& msg );
  };
}