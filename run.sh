echo "================================================"
echo " ROS 2 Jazzy - build package Bocbot"
echo " Ubuntu 24.04 Noble"
echo "================================================"

colcon build --symlink-install --packages-select bocbot
source install/setup.bash

