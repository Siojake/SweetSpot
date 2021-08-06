cmake_minimum_required(VERSION 2.8.3)
project(spot_control)

find_package(
  catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  i2cpwm_board
#  message_generation
  )

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES ${PROJJECT_NAME}
#  CATKIN_DEPENDS roscpp std_msgs message_runtime i2cpwm_board
  )

include_directories(
#  include
  ${catkin_INCLUDE_DIRS}
  ${i2cpwm_board_INCLUDE_DIRS}
  )


add_executable( spot_control_node
  src/Body.cpp
  src/Leg.cpp
  src/Servo.cpp
  src/spot_control.cpp
  )



target_link_libraries( spot_control_node
  ${catkin_LIBRARIES}
  ${roscpp_LIBRARIES}
  )



