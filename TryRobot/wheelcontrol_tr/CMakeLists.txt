cmake_minimum_required(VERSION 2.8.3)
project(wheelcontrol_tr)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
   LIBRARIES wheelcontrol_tr
   CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(wheelcontrol_tr_node src/wheelcontrol_tr.cpp)
add_dependencies(wheelcontrol_tr_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(wheelcontrol_tr_node ${catkin_LIBRARIES})
