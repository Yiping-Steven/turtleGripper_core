# Documentation

Created: Mar 23, 2021 6:50 PM
Engineers: Irina Bataeva, Yiping Zheng, jkwon35@jh.edu
Priority: P5
Sprint: Sprint_Doc
Status: In Progress
Type: Task 🔨

[Manipulation Module](Documentation%20a5e2e48dd3c44cdbaa33fcf2c006f4b3/Manipulation%20Module%204565e828a5b14ec3a7e5c62458204320.md)

[Computer Vision Module](Documentation%20a5e2e48dd3c44cdbaa33fcf2c006f4b3/Computer%20Vision%20Module%20f52732c5a1444c9b81627a8ba8fceca6.md)

[Task Management Module](Documentation%20a5e2e48dd3c44cdbaa33fcf2c006f4b3/Task%20Management%20Module%206ad492617b6d4727886bc3eb88058948.md)

[Navigation Module](Documentation%20a5e2e48dd3c44cdbaa33fcf2c006f4b3/Navigation%20Module%2007b25084a9ce4012a7ec1cd7ccc2474c.md)

# A. Installation

 0. ROS is our preliminary requirement.

1. Here's our code [repo](https://github.com/Yiping-Steven/turtleGripper.git), put it somewhere and built.

```jsx
mkdir -p ~/projects/tgrip_ws/src
cd ~/projects/tgrip_ws/src
catkin init
git clone https://github.com/Yiping-Steven/turtleGripper.git
catkin build
```

2. We need these drivers to drive the robot hardware (kobuki base, lidar and the manipulator).

```jsx
sudo apt install ros-kinetic-turtlebot-bringup # to drive the turtlebot base
sudo apt install ros-kinetic-urg-node # to drive the lidar
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
roslaunch tgrip_nav_srvcli amcl_appartment.launch
```

or if you built your own simulated or real world environment and want to create a map:

```jsx
roslaunch tgrip_nav_srvcli gmapping_demo.launch
```

### 4. For visualization

```jsx
roslaunch tgrip_nav_srvcli view_navigation.launch
```

### 5. Start the manager node