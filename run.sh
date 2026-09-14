echo "================================================"
echo " ROS 2 Jazzy - Build package Bocbot"
echo " Ubuntu 24.04 Noble"
echo "================================================"

rm -rf build/bocbot
rm -rf install/bocbot
colcon build --packages-select bocbot --symlink-install


