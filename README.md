# TurtleGripper
A mobile manipulator robot designed for household usage. Let's realize the dream of throwing your socks freeeeeely! For more details, please visit our [wiki page.](https://github.com/Yiping-Steven/turtleGripper_core/wiki)

# Demos
By May 13th, the turtleGripper (or "tgrip") can do:
1. Building a map both in simulation[[1]](https://kapwi.ng/c/ORo6ed82) and in real world[[2]](https://rsp-vja7544.slack.com/files/U01PKF49S1G/F021Y2Q1599/image_from_ios.mov).
2. Navigating in the known environment, both in simulation[[3]](https://kapwi.ng/c/UrDYlY7T) and in real world[4]
3. Recognizing targets. (Both in simulation [[3]](https://kapwi.ng/c/UrDYlY7T) and in real world [[5]](https://kapwi.ng/c/kVlHK2oW) )
4. Picking up the target from the ground. ( in real world [[6]](https://kapwi.ng/c/rKTLSy1X), the simulation has an [issue](https://github.com/Yiping-Steven/turtleGripper_core/issues/2)  )

Originated from a course project of  EN.530.707 **Robot System Programming**.

**Mentor**: [Simon Leonard](https://www.cs.jhu.edu/~sleonard/)

**Student**: Yiping Zheng, Irina Bataeva, Jian Kwon

@ LCSR, WSE, Johns Hopkins University.

# A. Installation

  0. ROS is our preliminary requirement.

 1. Here's our [core repo](https://github.com/Yiping-Steven/turtleGripper_core.git) and [3rd-Party repo](https://github.com/Yiping-Steven/turtleGripper_3rdParty.git), put it somewhere and build.

```shell
mkdir -p ~/projects/tgrip_ws/src
cd ~/projects/tgrip_ws/src
catkin init
git clone https://github.com/Yiping-Steven/turtleGripper_core.git
git clone https://github.com/Yiping-Steven/turtleGripper_3rdParty.git
catkin build
```

2. We need these drivers to drive the robot hardware (kobuki base, lidar and the manipulator).

```shell
sudo apt install ros-kinetic-turtlebot-bringup # to drive the turtlebot base
sudo apt install ros-kinetic-urg-node # to drive the lidar
sudo apt-get install ros-kinetic-ros-controllers ros-kinetic-gazebo* ros-kinetic-moveit* ros-kinetic-industrial-core # to drive manipulator
sudo apt-get install ros-kinetic-aruco-ros #to detect AR tags
```

# B. Configuration

1. open your [shell config file](https://landoflinux.com/linux_bash_configuration_files.html), for example:

```shell
vim ~/.bashrc
```

2. Add these environment variables to your shell config file.

```shell
export TURTLEBOT_3D_SENSOR=kinect
export TURTLEBOT_MAP_FILE=$(PROJECT_PATH)/tgrip_ws/src/core/nav_module/tgrip_nav_srvcli/map/my_map.yaml
```

3. import the tgrip packages to your shell environment.

```shell
source /home/project/tgrip_ws/devel/setup.bash
```

# C. Running

### 0. Start ROS

```shell
roscore
```

### 1. To run the simulation environment or the hardware

```bash
roslaunch tgrip_gazebo simulation.launch gui:=false #for silence, specify "gui" 
```

or if you have all the hardware connected and want to do it in real world:

```shell
roslaunch tgrip_nav_srvcli minimal.launch
```

### 2. Load the robot_description

```shell
roslaunch tgrip_description tgrip_description.launch
```
We have an issue that forces us to load robot_description separately.

### 3. For SLAM

We have prebuilt map for the gazebo simulation environment (make sure you choose simulation in Step 1):

```shell
roslaunch tgrip_nav_srvcli amcl_demo.launch #
```

or if you choose the real world in Step 1 and have a map for the environment (eg. my appartment)

```shell
roslaunch tgrip_nav_srvcli amcl_apartment.launch
```

or if you built your own simulated or real world environment and want to create a map.  (It doesn't matter what you chose in Step 1)

```shell
roslaunch tgrip_nav_srvcli gmapping_demo.launch
```

Control the turtlebot with keyboard
```shell
roslaunch turtlebot_teleop keyboard_teleop.launch
```

### 4. For cube detection

```shell
roslaunch tgrip_cv_cube find_cube.launch
```

### 5. For visualization (RViz)

```shell
roslaunch tgrip_nav_srvcli view_navigation.launch
```

### 6. Start the manager node  (not fully tested yet)

First launch the server. (to be automated)
```
rosrun tgrip_mng_srvcli taskServer_node
```
Then launch the user interface:
```
rosrun tgrip_mng_srvcli taskClient_node
```
