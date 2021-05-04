# turtleGripper
A household robot that is eventually going to pick up socks on the floor for you. 
Let's go for it!

# Dependencies
0. ROS
sudo apt install ros-kinetic-full-desktop

1. LIDAR driver
sudo apt install ros-kinetic-urg-*

2. turtlebot driver
sudo apt install ros-kinetic-turtlebot-bringup

## Basic Rules to follow:

### 1. Maintain the architecture
 For architecture, please refer to the [lucid chart](https://lucid.app/lucidchart/invitations/accept/inv_351e3888-64df-4e84-be68-46a606697e94 ), update the chart if you want to add new services/msgs/actions/topics
### 2. Creating new packages
 This repo is to hold all the ros packages we worte ourselves. Create whatever packages you like under your own module directory. You can depend on other packages and use them in your own workspace, but don't put them into this repo. We can later add these 3rd-party packages using git submodule method, but keep them out of this repo for now, this well help our code to be more independent.
### 3. Naming conventions
 For the name of packages, please use tgrip_${PACKAGEAME}_type. For example, tgrip_nav_actions, tgrip_nav_msgs, tgrip_nav_srvcli, tgrip_nav_subpub, etc.
 For the taskMng_module, which we will code together intensively, I've created names for most of the functions and variables, please feel free to ask me if you have any doubts about them. Other than that, it doesn't really matter what style you use in your own packages, so **feel free to name your own functions and variables with your favorite names**. With that said, I personally feel it beneficial to stick with some coding convention of the mainstream, the way I name funcitons and variables follows the [google style guidance](https://google.github.io/styleguide/cppguide.html) I posted in the #resource channel (with a few exceptions).  


## Explanation of two ciritcal data we are going to need.

Following is a quote from Q&A between Yiping and Jian:
### 1. geometry_msgs/Pose3D optimalGrippingPose
or the so-called sweet spot, is a result of some trade-off between [CV] and [Man], making that relative pose both easy for the [CV] to detect and provide visual feedback and for [Man] to have a high success rate of the gripping.  We will rely on it intensively when [Nav] is computing a goal pose of the global exploration.
### 2. geometry_msgs/Mesh reachableWorkspace
 this will serve as a criteria for the process management. When the globalExploration stops, tGripper has to use some criteria to decide whether it should do a go-pick-go-place subTask or doing a local searching subTask. Thanks to the data from rechableWorkspace, we can easily check whether the target has already enetered the reachableWorkSpace of the robot or not  and this is an ideal criteria for this kind of examination,  not to mention its potential usage for collision checking. Although it's not as important as the first one, optimalGrippingPose (since there exists some ways to circumvent from this), believe me, this is the right thing to do and choosing other ways are going to make the logic of the task more complex for all of us and even cause our separate tasks to be coupling.

