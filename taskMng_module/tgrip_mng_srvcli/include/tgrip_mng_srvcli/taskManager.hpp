#include <ros/ros.h>

// actions
//[Nav]
// #include <tgrip_mng_actions/navMoveAction.h>
// #include <tgrip_mng_actions/navExploreAction.h>
// #include <tgrip_mng_actions/navManeuverAction.h>
// #include <tgrip_mng_actions/navReturnAction.h>
//[Man]
// #include <tgrip_mng_actions/manPickAction.h>
// #include <tgrip_mng_actions/manPlaceAction.h>
#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>
//[Task]
#include <tgrip_mng_actions/tskManageAction.h>
#include <tgrip_mng_actions/tskTimedPatrolAction.h>
#include <tgrip_mng_msgs/serviceQuery.h>
// msgs
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_base_msgs/MoveBaseActionGoal.h>
#include <tgrip_cv_msgs/Cube.h>
// utilities
#include <memory>
#include <tf/tf.h>
#include <time.h>
#include <math.h>
#include <string>
#include <vector>

// macros
#define PI 3.1415926

namespace tgrip{

    class taskManager{

        /* ***********************************types definition***********************************  */
        //Tasks
        // typedef actionlib::SimpleActionServer<tgrip_mng_actions::tskMeteredPatrolAction> tskMeteredPatrolActionServer;
        typedef actionlib::SimpleActionServer<tgrip_mng_actions::tskTimedPatrolAction> tskTimedPatrolActionServer;
        // typedef actionlib::SimpleActionServer<tgrip_mng_actions::tskFetchingAction> tskFetchingActionServer;
        // typedef actionlib::SimpleActionServer<tgrip_mng_actions::tskMappingAction> tskMappingActionServer;
        // subTasks
        // typedef actionlib::SimpleActionClient<tgrip_mng_actions::navMoveAction> navMoveActionClient;
        // typedef actionlib::SimpleActionClient<tgrip_mng_actions::navExploreAction> navExploreActionClient;
        // typedef actionlib::SimpleActionClient<tgrip_mng_actions::navManeuverAction> navManeuverActionClient;
        // typedef actionlib::SimpleActionClient<tgrip_mng_actions::navReturnAction> navReturnActionClient;
        // typedef actionlib::SimpleActionClient<tgrip_mng_actions::manPlaceAction> manPlaceActionClient;
        // typedef actionlib::SimpleActionClient<tgrip_mng_actions::manPickAction> manPickActionClient;

    private:

        /* ***********************************Objects: All series***********************************  */
        ros::NodeHandle nh;
        // timer
        time_t timerMissionStart; // unit in seconds
        time_t timerPhase1;
        time_t timerPhase2;
        time_t timerPhase3;
        // meter
        double distMeasure; // unit in meters
        // map
        // pcl::pointCloud roomMap;
        // software status
        geometry_msgs::Pose2D poseCurrent; // store the real time pose.
        // geometry_msgs::Pose3D optimalGrippingPose;// obtaining from workspace calculation
        bool hasDoneMapping;
        // hardware status 
        bool batteryStatus;

        // task related
        // std::vector<tgrip_cv_msgs::target>  targetList; // create a target message type that stores the pose and certainty 
        ros::Publisher pubCmd; // for publishing movement command
        ros::Subscriber subOdom; // for updating the real time pose.
        ros::ServiceServer taskServer;

        /* ***********************************Objects: Task-oriented***********************************  */

        // action servers, corresponding to each service tGrip can provide, responding to different user requests. 
        // std::unique_ptr<tskMappingActionServer> tskMappingASPtr; 
        // std::unique_ptr<tskFetchingActionServer> tskFetchingASPtr; 
        // std::unique_ptr<tskMeteredPatrolActionServer> tskMeteredPatrolASPtr; 
        std::unique_ptr<tskTimedPatrolActionServer>   tskTimedPatrolASPtr;
        // std::unique_ptr<tskManagerActionServer> tskMngASPtr;// this server manage all the above

        // all clients
        // three navigation related
        // std::unique_ptr<navExploreActionClient> navExploreACPtr; // [Nav] doing global exploration
        // std::unique_ptr<navManeuverActionClient> navManeuverACPtr; // [Nav] doing local maneuver
        // std::unique_ptr<navReturnActionClient> navReturnACPtr; // [Nav] returning to bace
        // two manipulation related
        // std::unique_ptr<manPlaceActionClient> manPlaceACPtr; // [Man] doing placing
        // std::unique_ptr<manPickActionClient> manPickACPtr; // [Man] doing picking
        // one computer vision related
        // std::unique_ptr<ros::ServiceClient > cvIdClientPtr; //[CV] requesting detection resulty

        // [Nav]
        // actions
        // tgrip_mng_actions::navExploreGoal navExploreGoal; // explore for a distance, time, location or a coverage (advanced) 
        // tgrip_mng_actions::navExploreFeedback navExploreFeedback;
        // tgrip_mng_actions::navExploreResult navExploreResult;
        // tgrip_mng_actions::navManeuverGoal navManeuverGoal; // maneuver for an angle range
        // tgrip_mng_actions::navManeuverFeedback navManeuverFeedback;
        // tgrip_mng_actions::navManeuverResult navManeuverResult;
        // tgrip_mng_actions::navReturnGoal navReturnGoal; // return to a base location
        // tgrip_mng_actions::navReturnFeedback navReturnFeedback;
        // tgrip_mng_actions::navReturnResult navReturnResult;
        // all series (Yiping, add whatever you want here.)

        // [Man]
        // actions
        // tgrip_mng_actions::manPickGoal manPickGoal; // pick for a 3D pose
        // tgrip_mng_actions::manPickFeedback manPickFeedback;
        // tgrip_mng_actions::manPickResult manPickResult;
        // tgrip_mng_actions::manPlaceGoal manPlaceGoal; // place for a 3D pose
        // tgrip_mng_actions::manPlaceFeedback manPlaceFeedback;
        // tgrip_mng_actions::manPlaceResult manPlaceResult;
        // all series (Jian, add whatever you want here.)

        // [CV]
        // all series (Irina, add whatever you want here)
        ros::Subscriber subCube;
        ros::Publisher cubeTargetPub;
        bool cubeFound;
        bool cubeInView;
        double cubeDistance = 0.30; //target distance to the cube
        move_base_msgs::MoveBaseActionGoal goal;
        geometry_msgs::PoseStamped targetPose;
        geometry_msgs::Point cubePoint;
      
    public:

        // functions, all series 
        taskManager(ros::NodeHandle& nh);  // framework done
        bool tskMngSrvCB( tgrip_mng_msgs::serviceQuery::Request &req,
                        tgrip_mng_msgs::serviceQuery::Response &res ); // almost done
        void odomCallback(const nav_msgs::Odometry::ConstPtr& odomMsgPtr); // done

        void cubeCallback( const tgrip_cv_msgs::Cube& cube_detect ); // in progress
        // functions, task oriented
        void tskTimedPatrolGoalCB();    // TODO, together
        // void tskMeteredPatrolGoalCB();    // TODO, together
        // void tskFetchingGoalCB();       // TODO, Yiping's assignment
        // void tskMappingGoalCB();        //TODO, Yiping's assignment

        // void tskGlobalNavigation(); // TODO, Yiping's and Irina's assignment
        // void tskLocalManeuver(); // TODO, Irina and Jian's assignment
        // void tskLocalManipulation(); // TODO, Jian's assignment        
    };
}
