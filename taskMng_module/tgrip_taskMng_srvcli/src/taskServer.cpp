#include <taskMng_srvcli/taskServer.hpp>
#include <taskMng_srvcli/subTaskServer.hpp>

tgrip_taskMng_server::tgrip_taskMng_server (ros::NodeHandle& nh): nh( nh ){

  subOdom = nh.subscribe("/odom", 1000, & taskMngServer::odomCallback, this);
  pubCmd = nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 10);
  taskMngServer = nh.advertiseService("tgrip_taskMng_server", &tgrip_tskMng_server::tskMngSrvCB, this);
}


void tgrip_taskMng_server::tskMngSrvCB( tgrip_taskMng_msgs::assignTask::Request &req,
                  tgrip_taskMng_msgs::assignTask::Response &res ){

  // switch between all sorts of tasks
  // in case tskTimedPatrolAction is requested:
  init(req);
  tskTimedPatrolASPtr.reset( new tskTimedPatrolActionServer(nh, "tskTimedPatrolActionServer", false) );
  tskTimedPatrolASPtr->registerGoalCallback( boost::bind( tgrip_taskMng_server::tskTimedPatrolGoalCB() ));
  tskTimedPatrolASPtr->start();

  // in case tskMeteredPatrolAction is requested:
  init(req);
  tskMeteredPatrolASPtr.reset( new tskMeteredPatrolActionServer(nh, "tskMeteredPatrolActionServer", false) );
  tskMeteredPatrolASPtr->registerGoalCallback( boost::bind( tgrip_taskMng_server::tskMeteredPatrolGoalCB() ));
  tskMeteredPatrolASPtr->start();

  // in case tskMappingAction is requested:
  init(req);
  tskMappingASPtr.reset( new tskMappingActionServer(nh, "tskMappingActionServer", false) );
  tskMappingASPtr->registerGoalCallback( boost::bind( tgrip_taskMng_server::tskMappingGoalCB() ));
  tskMappingASPtr->start();
  
  // in case tskFetchingAction is requested:
  init(req);
  tskFetchingASPtr.reset( new tskFetchingActionServer(nh, "tskFetchingActionServer", false) );
  tskFetchingASPtr->registerGoalCallback( boost::bind( tgrip_taskMng_server::tskFetchingGoalCB() ));
  tskFetchingASPtr->start();

}


void tgrip_taskMng_server::tskTimedPatrolGoalCB(){
  // 1. tskGlobalNavigation()
  /*Call [Nav], from its base, start exploring. 
    Random walking is the bottom line, but I'll try to be inclusive. (Use AR tags)
    While [Nav] is doing global exploring, call [CV] once in a while to find target boxes.
         2.a In case [CV] founds a target but not very sure, return a pose. 
              Send that 2D pose to [Nav] and [Nav] will record it  (hereby known as "uncertain" pose) 
              and check later.
         2.b In case [CV] founds a target and is sure, return a pose. 
              Send that pose to [Nav]. [Nav] will pasue the exploration, 
              and start a go-pick-go-place subTask immediately. 
              Check this pose against the saved "uncertain" poses. 
              If the pose is within R =20 cm radius of any "uncertain" poses, delete that "uncertain" pose.
         2.c In case [CV] founds nothing. 
            Continue global exploration until one of the exit conditions is satisfied.
    */
  // 2. tskLocalManeuver()
   	    // Record the current location. 
        // Call [Nav] to go to the target location.
        // If [Nav] reported Goal Reached, Call [CV] to check if target can still be found. 
        // If not, Call [Nav] to do a local searching. Finely adjust the pose, prepare for manipulation 
        // (basically what this does is rotate along z axis and detect )
        // If found, check if it's within the reach of [Manip]. 
        // If within, Call [Manip] to start picking. 
        // If still not found, abandoned the task. 

  // 3. tskLocalManipulation(); 
      // do the picking.
      // If succeeded, Call [Nav] to return to base and do a similar reverse placing
      // Call [Nav] to return to the original "current location" at the beginning.	
      //Exit go-pick-go-place subtaask and	Resume the global exploration process


}




void tgrip_taskMng_server::tskMeteredPatrolGoalCB(){
  // very similar to tskTimedPatrolGoalCB, difference is merely the termination condition
};

void tgrip_taskMng_server::tskMeteredPatrolGoalCB(){};


void tgrip_taskMng_server::tskFetchingGoalCB(){
  std::cout << "taskMngServer::goalCB" << std::endl;
  goal = *(as->acceptNewGoal());
  std::cout << "New Goal Received:  ("<< goal.goal.poseGoal.x << ", "
                                      << goal.goal.poseGoal.y << ", "
                                      << goal.goal.poseGoal.theta << " ) "
                                      << std::endl;
  time(&timerMissionStart);
  distMeasure = 0;

  
  double theta1 = atan2(goal.goal.poseGoal.y - poseCurrent.y, goal.goal.poseGoal.x - poseCurrent.x ); // unit in rad
  double thetaDiffOrg = theta1 - poseCurrent.theta; // CCW +, CW -
  double thetaDiff = thetaDiffOrg;
  feedback.feedback.feedbackCode = "Phase 1: Stand at the current position and rotate " 
                                    + std::to_string( thetaDiffOrg ) + " radians.";
  feedback.feedback.prevTarget = 0;
  feedback.feedback.nextTarget = theta1;
  as->publishFeedback(feedback);  
  geometry_msgs::Twist cmdVelTwist;
  while ( abs(thetaDiff) > goal.goal.angTol){

    cmdVelTwist.angular.z = 1 * thetaDiff/abs(thetaDiffOrg); // CCW +, CW -
    pubCmd.publish(cmdVelTwist);
    thetaDiff = theta1 - poseCurrent.theta;
  }
  cmdVelTwist.angular.z = 0;
  pubCmd.publish(cmdVelTwist);


  double xDiff = goal.goal.poseGoal.x - poseCurrent.x;
  double yDiff = goal.goal.poseGoal.y - poseCurrent.y;
  double pathLenOrg = sqrt(pow(xDiff,2)+pow(yDiff,2));
  double pathLen = pathLenOrg;

  // feedback.feedback.feedbackCode = "Phase 2: Move straight forward."
  //                                   + string( pathLenOrg ) + " meters.";
  // feedback.feedback.prevTarget = thetaDiff;
  // feedback.feedback.nextTarget = pathLenOrg;

  as->publishFeedback(feedback);

  while ( pathLen > goal.goal.posTol){
    cmdVelTwist.linear.x = 0.2; // CCW +, CW -
    pubCmd.publish(cmdVelTwist);
    xDiff = goal.goal.poseGoal.x - poseCurrent.x;
    yDiff = goal.goal.poseGoal.y - poseCurrent.y;
    pathLen = sqrt(pow(xDiff,2)+pow(yDiff,2));
  }
  cmdVelTwist.linear.x = 0; // CCW +, CW -
  pubCmd.publish(cmdVelTwist);

  feedback.feedback.feedbackCode = "Phase 3: Stand at the current position and rotate.";
  as->publishFeedback(feedback);  
  thetaDiffOrg = goal.goal.poseGoal.theta - poseCurrent.theta; // CCW +, CW -
  thetaDiff = thetaDiffOrg;
  while ( abs(thetaDiff) > goal.goal.angTol){
    cmdVelTwist.angular.z = 1 * thetaDiff/abs(thetaDiffOrg); // CCW +, CW -
    pubCmd.publish(cmdVelTwist);
    thetaDiff = theta1 - poseCurrent.theta;
    time(&timerPhase3);
  }
  cmdVelTwist.angular.z = 0;
  pubCmd.publish(cmdVelTwist);

  float timeElapsed = difftime( timerPhase3, timerMissionStart); 
  result.result.resultCode = "Time elapsed: " + std::to_string(timeElapsed);
  // as->publishResult(result);
  std::cout<<"succeed" << std::endl;
  as->setSucceeded();
  
};


void tgrip_taskMng_server::odomCallback(const nav_msgs::Odometry::ConstPtr& odomMsgPtr){
  // convert the Odometry into Pose2D
  tf::Quaternion q( odomMsgPtr->pose.pose.orientation.x,
                    odomMsgPtr->pose.pose.orientation.y,
                    odomMsgPtr->pose.pose.orientation.z,
                    odomMsgPtr->pose.pose.orientation.w);
  tf::Matrix3x3 m(q);
  double roll, pitch, yaw;
  m.getRPY( roll, pitch, yaw);
  poseCurrent.theta = yaw;
  poseCurrent.x = odomMsgPtr->pose.pose.position.x;
  poseCurrent.y = odomMsgPtr->pose.pose.position.y;
  std::cout << std::setw(2);
  std::cout << poseCurrent.x<< ", "
            << poseCurrent.y << ", " 
            << poseCurrent.theta << "  %%odomCallback" << std::endl;
}