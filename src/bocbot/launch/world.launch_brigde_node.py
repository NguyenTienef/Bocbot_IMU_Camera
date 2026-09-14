from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os
import xacro


def generate_launch_description():
    pkg_bocbot = get_package_share_directory('bocbot')
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')

    world_path = os.path.join(pkg_bocbot, 'worlds', 'bocbot_office.world')
    xacro_path = os.path.join(pkg_bocbot, 'urdf', 'bocbot.urdf.xacro')

    robot_description_config = xacro.process_file(xacro_path)
    robot_description = {'robot_description': robot_description_config.toxml()}

    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ros_gz_sim, 'launch', 'gz_sim.launch.py')
        ),
        launch_arguments={'gz_args': f'-r {world_path}'}.items()
    )

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[robot_description]
    )

    # spawn near the building's interior (world file's building was re-centered near origin)
    spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=['-topic', 'robot_description', '-name', 'bocbot',
                   '-x', '0.0', '-y', '0.0', '-z', '0.2'],
        output='screen'
    )

    # bridge gz topics <-> ROS 2 topics for the camera and lidar
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            '/world/default/model/bocbot/link/camera/sensor/camera_sensor/image'
            '@sensor_msgs/msg/Image[gz.msgs.Image',
            '/world/default/model/bocbot/link/camera/sensor/camera_sensor/camera_info'
            '@sensor_msgs/msg/CameraInfo[gz.msgs.CameraInfo',
            '/world/default/model/bocbot/link/hokuyo/sensor/head_hokuyo_sensor/scan'
            '@sensor_msgs/msg/LaserScan[gz.msgs.LaserScan',
            '/odom@nav_msgs/msg/Odometry[gz.msgs.Odometry',
            '/tf@tf2_msgs/msg/TFMessage[gz.msgs.Pose_V',
            '/cmd_vel@geometry_msgs/msg/Twist]gz.msgs.Twist',
            '/model/bocbot/joint_state@sensor_msgs/msg/JointState[gz.msgs.Model',
        ],
        remappings=[
            ('/world/default/model/bocbot/link/camera/sensor/camera_sensor/image',
             '/bocbot/camera/image'),
            ('/world/default/model/bocbot/link/camera/sensor/camera_sensor/camera_info',
             '/bocbot/camera/camera_info'),
            ('/world/default/model/bocbot/link/hokuyo/sensor/head_hokuyo_sensor/scan',
             '/bocbot/scan'),
            ('/model/bocbot/joint_state', '/joint_states'),
        ],
        output='screen'
    )

    return LaunchDescription([
        gz_sim,
        robot_state_publisher,
        spawn_entity,
        bridge,
    ])