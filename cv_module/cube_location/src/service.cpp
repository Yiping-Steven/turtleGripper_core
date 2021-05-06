#include<tf/transform_broadcaster.h>
#include<tf/transform_listener.h>
#include<ros/ros.h>
#include<string>
#include<vector>
#include<deque>
#include<math.h>
#include<geometry_msgs/Point.h>



int main( int argc, char** argv ){

  ros::init( argc, argv, "service_node" );
  ros::NodeHandle nh;
  ros::NodeHandle nhp("~");
  tf::TransformListener listener;
  
  ros::Rate rate( 10.0 );
 
  std::string world_name;
  nhp.getParam( "world_name", world_name);
  
  ros::Publisher cube_publisher = nh.advertise<geometry_msgs::Point>("cube_position", 10); 
  vector<float> res {0.0 ,0.0, 0.0};
  deque<vector<float> cubeVec;
  std::cout<<"Publisher running"<<std::endl;
  
  while (nh.ok()){
    std::cout<<"blah"<<std::endl;
    geometry_msgs::Point cube_pose;

    //If the length of deque less than 5, return nans
    if (cubeVec.size()) < 5 {
        cube_pose.x = nanf("");
        cube_pose.y = nanf("");
        cube_pose.z = nanf("");  
      }
    else {
    //Get the Average Position from the vector
      for (int i = 0; i < cubeVec.size(); i++){
        for (int k = 0; i<3; i++){
    	  res[k] = res[k] + cubeVec[i][k]
      }
      for int i=0; i<res.size(); i++){
        res[i] = res[i]/res.size()
      }
        cube_pose.x = res[0];
        cube_pose.y = res[1];
        cube_pose.z = res[2];    
    }

    cube_publisher.publish(cube_pose);

    // Add things to the deque 
    tf::StampedTransfor transform;
    listener.waitForTransform( world_name, "/cube", ros::Time::now(), ros::Duration(1.0));
    listener.lookupTransform( world_name, "/cube", ros::Time::now(), transform);
    vector<float> cube_vec {transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin.z()};
    cubeVec.push_back(cube_vec);
    //If deque has more than 5 items, pop the oldest
    if (cubeVec.size() > 5){
      cubeVect.pop_front();
    }
    
    ros::spinOnce();
    rate.sleep();
  }

  return 0;

};
