#include<tf/transform_broadcaster.h>
#include<tf/transform_listener.h>
#include<ros/ros.h>
#include<string>
#include<vector>
#include<deque>
#include<math.h>
#include<geometry_msgs/Point.h>


int main( int argc, char** argv ){

  ros::init( argc, argv, "publisher_node" );
  ros::NodeHandle nh;
  ros::NodeHandle nhp("~");
  tf::TransformListener listener;
  ros::Rate rate( 1 );
  tf::StampedTransform transform;
  
  std::string world_name;
  nhp.getParam( "world_name", world_name);
  
  ros::Publisher cube_publisher = nh.advertise<geometry_msgs::Point>("cube_position", 10);
  std::cout<<"Cube position advertised"<<std::endl;
  
  
  std::deque<std::vector<double>> cubeVec;//Buffer of the pose
  int nDec = 10; //Buffer size
  bool cubeNotFound = true;
  while (nh.ok()){

    std::vector<double> res {0.0 ,0.0, 0.0};
    //If the cube hasn't been seen yet, return nans
    if (cubeNotFound) {
      //Assign nan to the response
      for (int i=0; i<res.size(); i++){
	res[i] = nanf("");
      }
  
    } else { 

      //Get the Average Position from the vector
      for (int i = 0; i < cubeVec.size(); i++) {
        for (int k = 0; k < res.size(); k++) {
    	  res[k] = res[k] + cubeVec[i][k];
	}
      }
    }

    //Published cube pose
    geometry_msgs::Point cube_pose;
    cube_pose.x = res[0];
    cube_pose.y = res[1];
    cube_pose.z = res[2];    
    cube_publisher.publish(cube_pose);
    //    std::cout<<cube_pose.x<<cube_pose.y<<cube_pose.z<<std::endl;
    //    std::cout<<res[0]<<res[1]<<res[2]<<std::endl;

    // Get the transform from map to cube
    std::cout<<"Waiting for the transform"<<std::endl;
    try{
      ros::Time now=ros::Time(0);
      listener.waitForTransform( world_name, "/cube", now, ros::Duration(2.0));
      listener.lookupTransform( world_name, "/cube", now, transform);

      // Add position from the transform to deque
      std::vector<double> cube_vec {transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()};
      cubeVec.push_back(cube_vec);
      std::cout<<"New cube pose acquired"<<std::endl;
      //Mark that the cube was found (only important for first sighting)
      cubeNotFound = false;
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
    }
        
    //If deque has more than 5 items, pop the oldest
    if (cubeVec.size() > nDec){
      cubeVec.pop_front();
      //      std::cout<<"Pushing back the cubeVect"<<std::endl;
    }
    
    // ros::spinOnce();
    rate.sleep();
  }

  return 0;

};
