# TurtleGripper
A mobile manipulator robot designed for household usage. Let's realize the dream of throwing your socks freeeeeely!
So far the turtleGripper (or "tgrip") can do:
1. Picking up the target from the ground. ( in real world [1] )
2. Building a map both in simulation[2](https://kapwi.ng/c/ORo6ed82) and in real world[3].
3. Navigating in the known environment, both in simulation[4] and in real world[5]
4. Recognizing targets. (Both in simulation [5] and in real world [6] )

Originated from a course project of  EN.530.707 **Robot System Programming**.

**Mentor**: [Simon Leonard](https://www.cs.jhu.edu/~sleonard/)

**Student**: Yiping Zheng, Irina Bataeva, Jian Kwon

@ LCSR, WSE, Johns Hopkins University.

# A. Installation

  0. ROS is our preliminary requirement.

 1. Here's our [core repo](https://github.com/Yiping-Steven/turtleGripper_core.git) and [3rd-Party repo](https://github.com/Yiping-Steven/turtleGripper_3rdParty.git), put it somewhere and build.

```jsx
mkdir -p ~/projects/tgrip_ws/src
cd ~/projects/tgrip_ws/src
catkin init
git clone https://github.com/Yiping-Steven/turtleGripper_core.git
git clone https://github.com/Yiping-Steven/turtleGripper_3rdParty.git
catkin build
```

2. We need these drivers to drive the robot hardware (kobuki base, lidar and the manipulator).

```jsx
sudo apt install ros-kinetic-turtlebot-bringup # to drive the turtlebot base
sudo apt install ros-kinetic-urg-node # to drive the lidar
sudo apt-get install ros-kinetic-ros-controllers ros-kinetic-gazebo* ros-kinetic-moveit* ros-kinetic-industrial-core # to drive manipulator
sudo apt-get install ros-kinetic-aruco-ros #to detect AR tags
```

# B. Configuration

1. open your [shell config file](https://landoflinux.com/linux_bash_configuration_files.html), for example:

```jsx
vim ~/.bashrc
```

2. Add these environment variables to your shell config file.

```jsx
export TURTLEBOT_3D_SENSOR=kinect
export TURTLEBOT_MAP_FILE=$(PROJECT_PATH)/tgrip_ws/src/core/nav_module/tgrip_nav_srvcli/map/my_map.yaml
```

3. import the tgrip packages to your shell environment.

```jsx
source /home/project/tgrip_ws/devel/setup.bash
```

# C. Running

### 0. Start ROS

```jsx
roscore
```

### 1. To run the simulation environment

```bash
roslaunch tgrip_gazebo simulation.launch gui:=false #for silence, specify "gui" 
```

or if you have all the hardware connected and want to do it in real world:

```jsx
roslaunch tgrip_nav_srvcli minimal.launch
```

### 2. Load the robot_description

```jsx
roslaunch tgrip_description tgrip_description.launch
```

### 3. For SLAM

We have prebuilt map for the gazebo simulation environment.

```jsx
roslaunch tgrip_nav_srvcli amcl_demo.launch
```

or if you choose the real world in step 2 and have a map for the environment (eg. my appartment)

```jsx
roslaunch tgrip_nav_srvcli amcl_apartment.launch
```

or if you built your own simulated or real world environment and want to create a map:

```jsx
roslaunch tgrip_nav_srvcli gmapping_demo.launch
```

### 4. For cube detection

```jsx
roslaunch tgrip_cv_cube find_cube.launch
```

### 5. For visualization (RViz)

```jsx
roslaunch tgrip_nav_srvcli view_navigation.launch
```

### 6. Start the manager node 
Still has some error by May 13, 2021.

```
rosrun tgrip_mng_srvcli client_node
```
