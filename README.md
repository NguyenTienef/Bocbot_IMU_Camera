# Bocbot ROS2 Gazebo Simulation

A comprehensive ROS2 simulation package for the Bocbot mobile robot using Gazebo, featuring a differential drive system, camera, LiDAR (Hokuyo), and teleoperation control.

## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Running the Simulation](#running-the-simulation)
- [Teleoperation Control](#teleoperation-control)
- [Robot Sensors](#robot-sensors)
- [Project Structure](#project-structure)
- [Troubleshooting](#troubleshooting)

## Features

- **Differential Drive System**: 4-wheel differential drive controller
- **Camera Sensor**: 800x800 RGB camera for vision tasks
- **LiDAR (Hokuyo)**: 720-sample laser scanner for environment perception
- **Gazebo 7+ Simulation**: Uses Gazebo with ROS 2 integration via ros_gz_bridge
- **Office Environment**: Pre-configured Bocbot office world for simulation
- **ROS 2 Humble**: Compatible with ROS 2 Humble distribution

## Prerequisites

- Ubuntu 24.04 LTS (or compatible)
- ROS2 Jazzy
- Gazebo 7+ (Gazebo Harmonic, not Gazebo Classic)
- Python 3.10+
- cmake 3.22+

## Installation

### 1. Install ROS2 Humble

```bash
sudo apt update
sudo apt install ros-humble-desktop
```

### 2. Install Required Dependencies

```bash
sudo apt install python3-colcon-common-extensions
sudo apt install ros-humble-ros-gz-sim
sudo apt install ros-humble-ros-gz-bridge
sudo apt install ros-humble-robot-state-publisher
```

### 3. Create and Setup Workspace

```bash
# Create workspace directory
mkdir -p ~/bocbot_ws/src
cd ~/bocbot_ws

# Clone or copy bocbot package to src/
# (Assuming bocbot is already in src/)

# Build the workspace
colcon build

# Source the setup script
source install/setup.bash

# Add to bashrc for persistent environment
echo "source ~/bocbot_ws/install/setup.bash" >> ~/.bashrc
```

## Building the Project

```bash
cd ~/bocbot_ws
colcon build
source install/setup.bash
```

## Running the Simulation

### Start the Gazebo Simulation

```bash
cd ~/bocbot_ws
source install/setup.bash
ros2 launch bocbot world.launch.py
```

This will:
1. Start Gazebo with the bocbot_office.world environment
2. Spawn the Bocbot robot at origin (0, 0, 0.2)
3. Launch robot_state_publisher for TF broadcasting
4. Start ros_gz_bridge to connect Gazebo topics to ROS 2 topics
5. Load the differential drive controller

**Available ROS 2 Topics:**
- `/bocbot/camera/image` - Camera RGB image (800x800)
- `/bocbot/camera/camera_info` - Camera intrinsic parameters
- `/bocbot/scan` - Hokuyo LiDAR scans (LaserScan)
- `/cmd_vel` - Command velocity (Twist) for robot control
- `/odom` - Odometry information
- `/joint_states` - Joint state updates for wheel positions
- `/tf` - Transform frames

## Teleoperation Control

### Control Robot with Keyboard

In a new terminal, run the teleoperation node:

```bash
source ~/bocbot_ws/install/setup.bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args --remap cmd_vel:=/cmd_vel
```

### Keyboard Controls

```
Linear/Rotational Movement:
   u    i    o
   j    k    l
   m    ,    .

Holonomic Mode (hold SHIFT):
   U    I    O
   J    K    L
   M    <    >

Vertical Movement:
   t : up (+z)
   b : down (-z)

Speed Control:
   q/z : increase/decrease max speeds by 10%
   w/x : increase/decrease only linear speed by 10%
   e/c : increase/decrease only angular speed by 10%

   CTRL-C : quit
```

**Note:** The robot uses differential drive, so horizontal holonomic movement may not be fully supported.

## Robot Sensors

### Camera Sensor

**Specifications:**
- Resolution: 800 x 800 pixels
- Format: RGB (R8G8B8)
- Update Rate: 30 Hz
- Field of View: ~80° horizontal
- Near Clip: 0.02 m, Far Clip: 300 m

**Topic:** `/bocbot/camera/image` and `/bocbot/camera/camera_info`

### LiDAR Sensor (Hokuyo)

**Specifications:**
- Samples: 720 horizontal samples per scan
- Scan Range: -90° to +90° (1.57 rad)
- Distance Range: 0.1 m to 30 m
- Update Rate: 40 Hz
- Noise Model: Gaussian (σ = 0.01 m)

**Topic:** `/bocbot/scan`

### Differential Drive System

**Specifications:**
- Wheel Separation: 0.30 m
- Wheel Radius: 0.10 m
- Odometry Publish Frequency: 100 Hz
- Control Topic: `/cmd_vel` (geometry_msgs/Twist)

**Topics:**
- `/odom` - Odometry (position and velocity estimates)
- `/joint_states` - Wheel joint positions and velocities
- `/tf` - Transform frames (odom → robot_footprint)

## Project Structure

```
Ros2_sim_pro/
├── src/
│   ├── bocbot/                    # Main Bocbot package
│   │   ├── launch/
│   │   │   └── world.launch.py    # Main launch file
│   │   ├── urdf/
│   │   │   ├── bocbot.urdf.xacro # Robot URDF (xacro format)
│   │   │   ├── bocbot.urdf       # Generated URDF file
│   │   │   └── bocbot.gazebo     # Gazebo plugins & sensors
│   │   ├── worlds/
│   │   │   └── bocbot_office.world # Gazebo world file
│   │   ├── include/              # Header files
│   │   ├── src/                  # Source files
│   │   ├── CMakeLists.txt        # CMake configuration
│   │   └── package.xml           # Package metadata
│   └── sjtu_drone_SLAM/          # SLAM components (optional)
├── build/                         # Build artifacts (generated)
├── install/                       # Installation files (generated)
├── log/                           # Build and runtime logs (generated)
├── build.sh                       # Build script
├── run.sh                         # Run script
├── .gitignore                     # Git ignore rules
└── README.md                      # This file
```

## Launch File Details

The `world.launch.py` file performs the following:

1. **Gazebo Simulation** - Launches Gazebo with `bocbot_office.world`
2. **Robot Description** - Processes URDF from xacro and creates robot model
3. **Robot State Publisher** - Publishes transform tree for robot kinematics
4. **Entity Spawning** - Spawns the Bocbot at origin (0, 0, 0.2)
5. **Sensor Bridging** - Connects Gazebo sensors to ROS 2 topics via ros_gz_bridge

## Troubleshooting

### Gazebo Not Starting

If Gazebo fails to start, ensure the correct packages are installed:

```bash
sudo apt install ros-humble-ros-gz-sim ros-humble-ros-gz-bridge
```

### Build Errors

If build fails during xacro processing:

```bash
# Clean and rebuild
cd ~/bocbot_ws
rm -rf build/ install/ log/
colcon build
```

### Sensor Topics Not Available

Verify ros_gz_bridge is running and check available topics:

```bash
# List all available topics
ros2 topic list

# Check for Gazebo bridge-related topics
ros2 topic list | grep bocbot
```

### Force Kill Gazebo

If Gazebo processes hang:

```bash
pkill -9 gz
pkill -9 gazebo
pkill -9 gzclient
pkill -9 gzserver
```

### Reset Robot Pose

To reset the robot to initial position in Gazebo, use the service:

```bash
ros2 service call /reset_simulation std_srvs/srv/Empty
```