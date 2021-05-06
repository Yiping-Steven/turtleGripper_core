# Open Manipulator

https://emanual.robotis.com/docs/en/platform/openmanipulator_x/overview/#overview

To run rivz with real robot,

roslaunch open_manipulator_controllers joint_trajectory_controller.launch sim:=false

We are using moveit RRT-Connect to generate manipulator trajectory.


roslaunch open_manipulator_controller open_manipulator_controller.launch
roslaunch open_manipulator_control_gui open_manipulator_control_gui.launch
