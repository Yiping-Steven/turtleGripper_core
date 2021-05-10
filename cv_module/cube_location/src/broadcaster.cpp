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
  
  
  std::deque<std::vector<double>> cubeVec;
  std::cout<<"Deque created empty:"<<std::endl;  
  
  while (nh.ok()){
    
    std::cout<<"Publisher running"<<std::endl;

    geometry_msgs::Point cube_pose;
    std::cout<< cubeVec.size()<<std::endl;
    std::vector<double> res {0.0 ,0.0, 0.0};
    //If the length of deque less than 5, return nans
    if (cubeVec.size() < 5) {
      
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
	//	std::cout<<res[k]<<std::endl;
      }
    }

    //Published cube pose
    cube_pose.x = res[0];
    cube_pose.y = res[1];
    cube_pose.z = res[2];    
    cube_publisher.publish(cube_pose);
    std::cout<<cube_pose.x<<cube_pose.y<<cube_pose.z<<std::endl;
    std::cout<<res[0]<<res[1]<<res[2]<<std::endl;
    // Get the transform from map to cube
    std::cout<<"Waiting for the transform"<<std::endl;
    try{
      ros::Time now=ros::Time(0);
      listener.waitForTransform( world_name, "/cube", now, ros::Duration(20.0));
      listener.lookupTransform( world_name, "/cube", now, transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
    }
    
    // Add position from the transform to deque
    std::vector<double> cube_vec {transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()};
    cubeVec.push_back(cube_vec);
    std::cout<<"New vector acquired"<<std::endl;
    
    //If deque has more than 5 items, pop the oldest
    if (cubeVec.size() > 5){
      cubeVec.pop_front();
      std::cout<<"Pushing back the cubeVect"<<std::endl;
    }
    
    // ros::spinOnce();
    rate.sleep();
  }

  return 0;

};
