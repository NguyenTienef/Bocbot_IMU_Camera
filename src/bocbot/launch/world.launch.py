from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

import os
import xacro


def generate_launch_description():

    # ============================================================
    # Package paths
    # ============================================================

    bocbot_pkg = get_package_share_directory('bocbot')
    ros_gz_sim_pkg = get_package_share_directory('ros_gz_sim')

    world_file = os.path.join(
        bocbot_pkg,
        'worlds',
        'bocbot_office.world'
    )

    xacro_file = os.path.join(
        bocbot_pkg,
        'urdf',
        'bocbot.urdf.xacro'
    )

    # ============================================================
    # Robot description
    # ============================================================

    robot_description = xacro.process_file(
        xacro_file
    ).toxml()

    robot_description_param = {
        'robot_description': robot_description
    }

    # ============================================================
    # Gazebo Harmonic
    # ============================================================

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                ros_gz_sim_pkg,
                'launch',
                'gz_sim.launch.py'
            )
        ),
        launch_arguments={
            'gz_args': f'-r {world_file}'
        }.items()
    )

    # ============================================================
    # Robot State Publisher
    # ============================================================

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',

        parameters=[
            robot_description_param,
            {
                'use_sim_time': True
            }
        ],

        output='screen'
    )

    # ============================================================
    # Spawn Bocbot into Gazebo
    # ============================================================

    spawn_robot = Node(
        package='ros_gz_sim',
        executable='create',

        arguments=[
            '-topic', 'robot_description',
            '-name', 'bocbot',

            '-x', '0.0',
            '-y', '0.0',
            '-z', '0.2'
        ],

        output='screen'
    )

    # ============================================================
    # Gazebo <-> ROS 2 bridge
    #
    # Camera:
    #   Gazebo  : /camera/image
    #   ROS 2   : /camera/image
    #
    # Camera info:
    #   Gazebo  : /camera/camera_info
    #   ROS 2   : /camera/camera_info
    #
    # Odometry:
    #   Gazebo  : /odom
    #   ROS 2   : /odom
    #
    # cmd_vel:
    #   ROS 2   : /cmd_vel
    #   Gazebo  : /cmd_vel
    #
    # Joint state:
    #   Gazebo  : /world/default/model/bocbot/joint_state
    #   ROS 2   : /joint_states
    #
    # TF is intentionally NOT bridged here.
    # robot_state_publisher handles TF.
    # ============================================================

    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',

        arguments=[

            # ----------------------------------------------------
            # Camera image
            # Gazebo -> ROS 2
            # ----------------------------------------------------

            '/camera/image'
            '@sensor_msgs/msg/Image'
            '[gz.msgs.Image',

            # ----------------------------------------------------
            # Camera info
            # Gazebo -> ROS 2
            # ----------------------------------------------------

            '/camera/camera_info'
            '@sensor_msgs/msg/CameraInfo'
            '[gz.msgs.CameraInfo',

            # ----------------------------------------------------
            # Odometry
            # Gazebo -> ROS 2
            # ----------------------------------------------------

            '/odom'
            '@nav_msgs/msg/Odometry'
            '[gz.msgs.Odometry',

            # ----------------------------------------------------
            # Velocity command
            # ROS 2 -> Gazebo
            # ----------------------------------------------------

            '/cmd_vel'
            '@geometry_msgs/msg/Twist'
            ']gz.msgs.Twist',

            # ----------------------------------------------------
            # Joint states
            # Gazebo -> ROS 2
            # ----------------------------------------------------

            '/world/default/model/bocbot/joint_state'
            '@sensor_msgs/msg/JointState'
            '[gz.msgs.Model',
        ],

        output='screen'
    )

    # ============================================================
    # Bocbot camera C++ node
    # ============================================================

    camera_node = Node(
        package='bocbot',
        executable='camera_node',

        parameters=[
            {
                'use_sim_time': True
            }
        ],

        output='screen'
    )

    # ============================================================
    # Launch all
    # ============================================================

    return LaunchDescription([
        gazebo,
        robot_state_publisher,
        spawn_robot,
        bridge,
        camera_node,
    ])