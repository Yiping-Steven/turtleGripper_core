#include <ros/ros.h>

// actions
#include <tgrip_nav_actions/moveAction.h>
#include <tgrip_nav_actions/exploreAction.h>
#include <tgrip_nav_actions/maneuverAction.h>
#include <tgrip_nav_actions/returnAction.h>

#include <tgrip_man_actions/pickAction.h>
#include <tgrip_man_actions/placeAction.h>

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


class tgrip_taskMng_server{

    typedef actionlib::SimpleActionServer<tgrip_taskMng_actions::tskMeteredPatrolAction> tskMeteredPatrolActionServer;
    typedef actionlib::SimpleActionServer<tgrip_taskMng_actions::tskTimedPatrolAction> tskTimedPatrolActionServer;
    typedef actionlib::SimpleActionClient<tgrip_nav_actions::navMoveAction> navMoveActionClient;
    typedef actionlib::SimpleActionClient<tgrip_nav_actions::navExploreAction> navExploreActionClient;
    typedef actionlib::SimpleActionClient<tgrip_nav_actions::navManeuverAction> navManeuverActionClient;
    typedef actionlib::SimpleActionClient<tgrip_nav_actions::navReturnAction> navReturnActionClient;
    typedef actionlib::SimpleActionClient<tgrip_man_actions::manPlaceAction> manPlaceActionClient;
    typedef actionlib::SimpleActionClient<tgrip_man_actions::manPickAction> manPickActionClient;

private:

    ros::NodeHandle nh;
    // timer
    time_t timerMissionStart; // unit in seconds
    time_t timerPhase1;
    time_t timerPhase2;
    time_t timerPhase3;
    // ......

    // all servers
    ros::ServiceServer taskMngServer; // serving the USER client
    std::unique_ptr<tgrip_taskMng_actions::meteredPatrolAction> tskMeteredPatrolASPtr; 
    std::unique_ptr<tgrip_taskMng_actions::timedPatrolAction>   tsktimedPatrolASPtr; // respond to different user requests. 
    // all clients
    std::unique_ptr<navExploreActionClient> navExploreACPtr; // [Nav] doing global exploration
    std::unique_ptr<navManeuverActionClient> navManeuverACPtr; // [Nav] doing local maneuver
    std::unique_ptr<navReturnActionClient> navReturnACPtr; // [Nav] returning to bace
    std::unique_ptr<manPlaceActionClient> manPlaceACPtr; // [Man] doing placing
    std::unique_ptr<manPickActionClient> manPickACPtr; // [Man] doing picking
    std::unique_ptr<ros::ServiceClient > cvIdClientPtr; //[CV] requesting detection resulty

    // [Nav]
    // actions
    tgrip_nav_actions::navExploreGoal navExploreGoal; // explore for a distance, time, location or a coverage (advanced) 
    tgrip_nav_actions::navExploreFeedback navExploreFeedback;
    tgrip_nav_actions::navExploreResult navExploreResult;
    tgrip_nav_actions::navManeuverGoal navManeuverGoal; // maneuver for an angle range
    tgrip_nav_actions::navManeuverFeedback navManeuverFeedback;
    tgrip_nav_actions::navManeuverResult navManeuverResult;
    tgrip_nav_actions::navReturnGoal navReturnGoal; // return to a base location
    tgrip_nav_actions::navReturnFeedback navReturnFeedback;
    tgrip_nav_actions::navReturnResult navReturnResult;
    // all series
    ros::Publisher pubCmd; // for publishing movement command
    ros::Subscriber subOdom; // for updating the real time pose.
    geometry_msgs::Pose2D poseCurrent; // store the real time pose.

    // [Man]
    // actions
    tgrip_nav_actions::manPickGoal manPickGoal; // pick for a 3D pose
    tgrip_nav_actions::manPickFeedback manPickFeedback;
    tgrip_nav_actions::manPickResult manPickResult;
    tgrip_nav_actions::manPlaceGoal manPlaceGoal; // place for a 3D pose
    tgrip_nav_actions::manPlaceFeedback manPlaceFeedback;
    tgrip_nav_actions::manPlaceResult manPlaceResult;
    // all series (Jian, add whatever you want here.)

    // [CV]
    std::vector<cv_msgs::target>  targetList; // create a target message type that stores the pose and certainty 
    // all series (Irina, add whatever you want here)

public:

    tgrip_taskMng_server(ros::NodeHandle& nh); 

    void init();

    void tskMeteredPatrolActionCB();
    
    void tskTimedPatrolActionCB();

    void tskGlobalNavigation(); // Yiping's assignment
    void tskLocalManeuver(); // Irina's assignment
    void tskLocalManipulation(); // Jian's assignment

    void odomCallback(const tgrip_nav_msgs::Odometry::ConstPtr& odomMsgPtr);
    
};