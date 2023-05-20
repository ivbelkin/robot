cmake_minimum_required(VERSION 2.8.3)
project(camera_driver)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  cv_bridge
  message_generation
  camera_calibration_parsers
)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
	include_directories(/usr/src/jetson_multimedia_api/include)
	link_directories(/usr/lib/aarch64-linux-gnu/tegra)
endif()

find_package(jetson-utils REQUIRED)
find_package(CUDA REQUIRED)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${CUDA_INCLUDE_DIRS}
)

include_directories(
  /usr/include/gstreamer-1.0
  /usr/include/glib-2.0
  /usr/include/libxml2
  /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include
  /usr/lib/x86_64-linux-gnu/gstreamer-1.0/include/
  /usr/lib/aarch64-linux-gnu/glib-2.0/include/
  /usr/lib/x86_64-linux-gnu/glib-2.0/include/
)
add_definitions(-DROS1)

add_executable(camera_driver src/camera_driver.cpp src/image_converter.cpp src/ros_compat.cpp)
target_link_libraries(camera_driver ${catkin_LIBRARIES} jetson-utils)
