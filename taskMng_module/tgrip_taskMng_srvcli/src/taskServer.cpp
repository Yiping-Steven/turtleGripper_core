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
  // 1. tskGlobalNavigation
  // 2. tskLocalManeuver
  // 3. tskLocalmanipulation();

}
void tgrip_taskMng_server::tskMeteredPatrolGoalCB(){};

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