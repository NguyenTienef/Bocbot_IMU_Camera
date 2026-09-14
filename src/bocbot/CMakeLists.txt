cmake_minimum_required(VERSION 3.8)

project(bocbot)

# ============================================================
# C++ standard
# ============================================================

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# ============================================================
# ROS 2 dependencies
# ============================================================

find_package(ament_cmake REQUIRED)

find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)

find_package(OpenCV REQUIRED)

find_package(xacro REQUIRED)


# ============================================================
# Generate URDF from Xacro
# ============================================================

file(GLOB xacro_files
  urdf/*.urdf.xacro
)

foreach(xacro_file ${xacro_files})

  string(REGEX MATCH
    "(.*)[.]xacro$"
    unused
    ${xacro_file}
  )

  set(output_filename ${CMAKE_MATCH_1})

  xacro_add_xacro_file(
    ${xacro_file}
    ${output_filename}
  )

  list(APPEND urdf_files
    ${output_filename}
  )

endforeach()


add_custom_target(
  media_files ALL
  DEPENDS ${urdf_files}
)


# ============================================================
# Camera node
# ============================================================

add_executable(camera_node
  src/camera.cpp
)

ament_target_dependencies(
  camera_node
  rclcpp
  sensor_msgs
  cv_bridge
)

target_link_libraries(
  camera_node
  ${OpenCV_LIBRARIES}
)


# ============================================================
# Depth Camera node
# ============================================================

add_executable(depth_camera_node
  src/depth_cam.cpp
)

ament_target_dependencies(
  depth_camera_node
  rclcpp
  sensor_msgs
  cv_bridge
)

target_link_libraries(
  depth_camera_node
  ${OpenCV_LIBRARIES}
)


# ============================================================
# A* Path Planning node
# ============================================================

add_executable(a_star_node
  src/A_star.cpp
)

ament_target_dependencies(
  a_star_node
  rclcpp
  nav_msgs
  geometry_msgs
)


# ============================================================
# RRT Path Planning node
# ============================================================

add_executable(rrt_node
  src/RRT.cpp
)

ament_target_dependencies(
  rrt_node
  rclcpp
  nav_msgs
  geometry_msgs
)


# ============================================================
# LIDAR SLAM node
# ============================================================

add_executable(lidar_slam_node
  src/lidar_slam.cpp
)

ament_target_dependencies(
  lidar_slam_node
  rclcpp
  sensor_msgs
  nav_msgs
  geometry_msgs
  tf2_ros
)


# ============================================================
# Install executables
# ============================================================

install(
  TARGETS
    camera_node
    depth_camera_node
    a_star_node
    rrt_node
    lidar_slam_node
  DESTINATION lib/${PROJECT_NAME}
)


# ============================================================
# Install package resources
# ============================================================

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY urdf
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY worlds
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include
)


# ============================================================
# Testing
# ============================================================

if(BUILD_TESTING)

  find_package(ament_lint_auto REQUIRED)

  ament_lint_auto_find_test_dependencies()

endif()


# ============================================================
# Package
# ============================================================

ament_package()