## Tutorials to create a GAZEBO simmulation 
- Set up environment
ROS2 humble
Gazebo + zacro + urdf
Rviz
```
sudo apt update
sudo apt install ros-humble-desktop
sudo apt install python3-colcon-common-extensions
```
# Tạo package
```
mkdir -p ~/bocbot_ws/src
cd ~/bocbot_ws
colcon build
source install/setup.bash

#thêm vào môi trường
echo "source ~/bocbot_ws/install/setup.bash" >> ~/.bashrc
```
# build file và chạy giả lập
```
cd ~/bocbot_ws
colcon build
source install/setup.bash
ros2 launch bocbot world.launch.py
```
# lệnh điều khiển teleop_key

```
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args   --remap cmd_vel:=/bocbot/cmd_vel
```
# Log:
```
This node takes keypresses from the keyboard and publishes them
as Twist/TwistStamped messages. It works best with a US keyboard layout.
---------------------------
Moving around:
   u    i    o
   j    k    l
   m    ,    .

For Holonomic mode (strafing), hold down the shift key:
---------------------------
   U    I    O
   J    K    L
   M    <    >

t : up (+z)
b : down (-z)

anything else : stop

q/z : increase/decrease max speeds by 10%
w/x : increase/decrease only linear speed by 10%
e/c : increase/decrease only angular speed by 10%

CTRL-C to quit
```

# lệnh clear srv gazebo tránh lỗi 
```
pkill -9 gzserver
pkill -9 gzclient
pkill -9 gazebo

```
# IMU 1
```
<gazebo reference="imu_link">
  <sensor name="imu_sensor" type="imu">
    <always_on>true</always_on>
    <update_rate>200</update_rate>

    <imu>
      <angular_velocity>
        <x><noise type="gaussian"><mean>0</mean><stddev>0.0002</stddev></noise></x>
        <y><noise type="gaussian"><mean>0</mean><stddev>0.0002</stddev></noise></y>
        <z><noise type="gaussian"><mean>0</mean><stddev>0.0002</stddev></noise></z>
      </angular_velocity>

      <linear_acceleration>
        <x><noise type="gaussian"><mean>0</mean><stddev>0.01</stddev></noise></x>
        <y><noise type="gaussian"><mean>0</mean><stddev>0.01</stddev></noise></y>
        <z><noise type="gaussian"><mean>0</mean><stddev>0.01</stddev></noise></z>
      </linear_acceleration>
    </imu>

    <plugin name="imu_plugin" filename="libgazebo_ros_imu_sensor.so">
      <ros>
        <namespace>/bocbot</namespace>
        <remapping>imu:=imu/data</remapping>
      </ros>
      <frame_name>imu_link</frame_name>
    </plugin>
  </sensor>
</gazebo>

```

```
  <!-- IMU BNO085 -->
  <gazebo reference="imu">
    <sensor name="imu_sensor" type="imu">
      <always_on>true</always_on>
      <update_rate>100.0</update_rate>
      <visualize>true</visualize>
      <topic>imu</topic>
      
      <plugin filename="libgazebo_ros_imu_sensor.so" name="imu_plugin">
        <ros>
          <namespace>/bocbot</namespace>
          <remapping>~/out:=imu/data</remapping>
        </ros>
        
        <initial_orientation_as_reference>false</initial_orientation_as_reference>
        <frame_name>imu</frame_name>
        
        <!-- Noise parameters for BNO085 -->
        <angular_velocity_stdev>0.0001</angular_velocity_stdev>
        <linear_acceleration_stdev>0.0001</linear_acceleration_stdev>
        <orientation_stdev>0.0001</orientation_stdev>
      </plugin>
    </sensor>
  </gazebo>
```