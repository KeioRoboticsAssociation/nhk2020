cmake_minimum_required(VERSION 2.8.3)
project(localization_4wds_tr)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  nav_msgs
  tf
)

catkin_package(
#  INCLUDE_DIRS include
   LIBRARIES localization_4wds_tr
   CATKIN_DEPENDS roscpp std_msgs nav_msgs tf
   DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(localization_4wds_tr_node src/localization_4wds_tr.cpp)
add_dependencies(localization_4wds_tr_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(localization_4wds_tr_node ${catkin_LIBRARIES})
