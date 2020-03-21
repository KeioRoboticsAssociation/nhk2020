cmake_minimum_required(VERSION 2.8.3)
project(joystick_pr)

find_package(
  catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

catkin_package(
  LIBRARIES joystick_pr
  CATKIN_DEPENDS roscpp std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(joystick_pr_node src/joystick_pr.cpp)
add_dependencies(joystick_pr_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(joystick_pr_node ${catkin_LIBRARIES})