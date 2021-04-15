#include <ros/ros.h>

// actions
#include <nav_actions/moveAction.h>
#include <nav_actions/exploreAction.h>
#include <nav_actions/maneuverAction.h>
#include <nav_actions/returnAction.h>

#include <man_actions/pickAction.h>
#include <man_actions/placeAction.h>

#include <actionlib/server/simple_action_server.h>
#include <actionlib/server/simple_action_client.h>

// msgs
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>

// utilities
#include <memory>
#include <tf/tf.h>
#include <time.h>
#include <math.h>
#include <string>
#include <vector>

// macros
#define PI 3.1415926


class taskMngServer{
    // task action server
    typedef actionlib::SimpleActionServer<taskMng_actions::meteredPatrolAction> tskMeteredPatrolActionServer;
    typedef actionlib::SimpleActionServer<taskMng_actions::timedPatrolAction> timedPatrolActionServer;
    // subtask action clients [Nav]
    typedef actionlib::SimpleActionClient<nav_actions::moveAction> navMoveActionClient;
    typedef actionlib::SimpleActionClient<nav_actions::exploreAction> navExploreActionClient;
    typedef actionlib::SimpleActionClient<nav_actions::maneuverAction> navManeuverActionClient;
    typedef actionlib::SimpleActionClient<nav_actions::returnAction> navReturnActionClient;
    // subtask action clients [Nav]
    typedef actionlib::SimpleActionClient<man_actions::placeAction> manPlaceActionClient;
    typedef actionlib::SimpleActionClient<man_actions::pickAction> manPickActionClient;

private:

    ros::NodeHandle nh;
    ros::ServiceServer taskMngSrv; // serving the USER client
    // timer
    time_t timerMissionStart; // unit in seconds
    time_t timerPhase1;
    time_t timerPhase2;
    time_t timerPhase3;
    // all servers
    std::unique_ptr<taskMng_actions::meteredPatrolAction> tskMeteredPatrolASPtr; // [Nav] doing global exploration
    std::unique_ptr<taskMng_actions::timedPatrolAction> timedPatrolASPtr; // [Nav] doing global exploration
    // all clients
    std::unique_ptr<navExploreActionClient> navExploreACPtr; // [Nav] doing global exploration
    std::unique_ptr<navManeuverActionClient> navManeuverACPtr; // [Nav] doing local maneuver
    std::unique_ptr<navReturnActionClient> navReturnACPtr; // [Nav] returning to bace
    std::unique_ptr<manPlaceActionClient> manPlaceACPtr; // [Man] doing placing
    std::unique_ptr<manPickActionClient> manPickACPtr; // [Man] doing picking
    std::unique_ptr<ros::ServiceClient > cvClientPtr; //[CV] requesting detection resulty

    // [Nav]
    // actions
    nav_actions::navExploreGoal navExploreGoal; // explore for a distance, time, location or a coverage (advanced) 
    nav_actions::navExploreFeedback navExploreFeedback;
    nav_actions::navExploreResult navExploreResult;
    nav_actions::navManeuverGoal navManeuverGoal; // maneuver for an angle range
    nav_actions::navManeuverFeedback navManeuverFeedback;
    nav_actions::navManeuverResult navManeuverResult;
    nav_actions::navReturnGoal navReturnGoal; // return to a base location
    nav_actions::navReturnFeedback navReturnFeedback;
    nav_actions::navReturnResult navReturnResult;
    // all series
    ros::Publisher pubCmd; // for publishing movement command
    ros::Subscriber subOdom; // for updating the real time pose.
    geometry_msgs::Pose2D poseCurrent; // store the real time pose.

    // [Man]
    // actions
    nav_actions::manPickGoal manPickGoal; // pick for a 3D pose
    nav_actions::manPickFeedback manPickFeedback;
    nav_actions::manPickResult manPickResult;
    nav_actions::manPlaceGoal manPlaceGoal; // place for a 3D pose
    nav_actions::manPlaceFeedback manPlaceFeedback;
    nav_actions::manPlaceResult manPlaceResult;
    // all series (Jian, add whatever you want here.)

    // [CV]
    std::vector<cv_msgs::target>  targetList; // create a target message type that stores the pose and certainty 
    // all series (Irina, add whatever you want here)



public:

    taskMngServer(ros::NodeHandle& nh); 

    void init();

    void run(tastkMng_srvcli::serveUser::Request &req, 
                taskMng_srvcli::serveUser::Response &res);

    void odomCallback(const nav_msgs::Odometry::ConstPtr& odomMsgPtr);
    
};