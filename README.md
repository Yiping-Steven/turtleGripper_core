# turtleGripper
A household robot that is eventually going to pick up socks on the floor for you. Let's go for it!

## Basic Rules to follow:

## 1. Maintain the architecture
 For architecture, please refer to the [lucid chart](https://lucid.app/lucidchart/invitations/accept/inv_351e3888-64df-4e84-be68-46a606697e94 ), update the chart if you want to add new services/msgs/actions/topics
## 2. Creating new packages
 This repo is to hold all the ros packages we worte ourselves. Create whatever packages you like under your own module directory. You can depend on other packages and use them in your own workspace, but don't put them into this repo. We can later add these 3rd-party packages using git submodule method, but keep them out of this repo for now, this well help our code to be more independent.
## 3. Naming conventions
 For the name of packages, please use tgrip_${PACKAGEAME}_type. For example, tgrip_nav_actions, tgrip_nav_msgs, tgrip_nav_srvcli, tgrip_nav_subpub, etc.
 For the taskMng_module, which we will code together intensively, I've created names for most of the functions and variables, please feel free to ask me if you have any doubts about them. Other than that, it doesn't really matter what style you use in your own packages, so feel free to name your own functions and variables with your favorite names. With that said, I personally feel it beneficial to stick with some coding convention of the mainstream, the way I name funcitons and variables follows the google style guidance I posted in the #resource channel (with a few exceptions).  


